/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include "qtiq_vrt.h"
#include "bit_ops.h"

#include <stdio.h>
#include <stdexcept>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <net/if.h>
#include <unistd.h>

namespace gr
{
namespace quadratiq
{
    
///////////////////////////////////////////////////////////////////
// VITA PACKET CONSTANTS FOR QUADRATIQ
typedef enum
{
    vrt_pkt_type_data=01,
    vrt_pkt_type_context=0x4,

    vrt_pkt_type_unknown=0x0000
} vrt_pkt_type_t;

////////////////////////////////////////
// HEADER
#define VITA_HEADER_MISC_DATA_CONST (0x46)
#define VITA_HEADER_MISC_CTXT_CONST (0x16)
// packet size
#define VITA_HEADER_PKT_SIZE_OFFSET (0)
#define VITA_HEADER_PKT_SIZE_LEN (16)
// packet count (modulo 16)
#define VITA_HEADER_PKT_COUNT_OFFSET (16)
#define VITA_HEADER_PKT_COUNT_LEN (4)
// packet C T R TSI TSF
#define VITA_HEADER_MISC_OFFSET (20)
#define VITA_HEADER_MISC_LEN (8)
// packet type
#define VITA_HEADER_PKT_TYPE_OFFSET (28)
#define VITA_HEADER_PKT_TYPE_LEN (4)
////////////////////////////////////////

#define VITA_HEADER_OFFSET (0)
#define VITA_STREAM_OFFSET (1)
#define VITA_TIMESTAMP_SEC_OFFSET (2)
#define VITA_TIMESTAMP_FRACT_SEC_HIGH_OFFSET (3)
#define VITA_TIMESTAMP_FRACT_SEC_LOW_OFFSET (4)
#define VITA_PAYLOAD_OFFSET (5)

// 1 for header, 1 for stream ID, 3 for timestamps, 1 for trailer=6
#define VITA_META_NUM_WORDS (6)

#define VITA_PKT_NUM_WORDS (VITA_META_NUM_WORDS+VITA_NUM_SAMPLES)

struct vrt_header_t
{
    vrt_pkt_type_t type;
    uint8_t pkt_count;
    uint16_t pkt_size;
};

struct vrt_packet_t
{    vrt_header_t header;
    uint32_t stream_id;
    float timestamp;

    uint32_t *p_data;
    uint16_t num_data_words;
};
///////////////////////////////////////////////////////////////////

qtiq_vrt::qtiq_vrt( std::string vrt_ip, uint32_t vrt_port, uint32_t dest_port, uint32_t base_stream_id )
{
    uint8_t buf[100000];
    int32_t bytes=0;
    
    create_socket( (const char*)(vrt_ip.c_str()), vrt_port, dest_port );

    d_base_id = base_stream_id;
    d_pkt_count = 255; // uninitialized
}

qtiq_vrt::~qtiq_vrt()
{
    if( d_vrt_fd > 0 )
    {
        close(d_vrt_fd);
    }
}

bool
qtiq_vrt::create_socket( const char *p_vrt_ip, uint32_t vrt_port, uint32_t dest_port )
{
    bool success=false;
    
    struct sockaddr_in vrt_sock; // radio generating VITA stream
    struct sockaddr_in dest_sock; // destination socket to receive on

    printf("creating UDP socket\n");
    if( (d_vrt_fd=socket(AF_INET, SOCK_DGRAM, 0)) == -1 )
    {
        throw std::runtime_error("unable to create VRT socket");
        return false;
    }

    // default the parameters
    memset( (char*)(&dest_sock), 0, sizeof(dest_sock) );
    memset( (char*)(&vrt_sock), 0, sizeof(vrt_sock) );

    // setup destination socket address
    dest_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_sock.sin_port = htons(dest_port);

    // setup source quadratiq address
    vrt_sock.sin_family = AF_INET;
    vrt_sock.sin_port = htons(vrt_port);
    vrt_sock.sin_addr.s_addr = inet_addr(p_vrt_ip);

    printf("binding socket...\n");
    if( bind(d_vrt_fd, (struct sockaddr*)(&dest_sock), sizeof(dest_sock)) == -1 )
    {
        throw std::runtime_error("unable to bind VRT socket");
        return false;
    }

    printf("connecting socket...\n");
    if( connect(d_vrt_fd, (const struct sockaddr*)(&vrt_sock), sizeof(vrt_sock)) != 0 )
    {
        throw std::runtime_error("unable to connect VRT socket");
    }

    printf("Socket created successfully!\r\n");

    return (success);
}

void
qtiq_vrt::receive_data_packet( int16_t *p_stream1, int16_t *p_stream2 )
{
    int32_t num_bytes=0;
    uint32_t buf[VITA_PKT_NUM_WORDS];
    vrt_packet_t pkt;
    bool received_stream[2] = {false};

    while( (received_stream[0] == false) ||
           (received_stream[1] == false) )
    {
        // receive a packet
        if( recv(d_vrt_fd, buf, VITA_PKT_NUM_WORDS*sizeof(uint32_t), 0) > 0 )
        {
            if( parse_vrt_packet( buf, &pkt ) == true )
            {
                // make sure it's a data packet with our stream ID
                if( (pkt.header.type == vrt_pkt_type_data) )
                {
                    // figure out which stream the data packet belongs to
                    if( (pkt.stream_id == d_base_id) &&
                        (received_stream[0] == false) ) 
                    {
                        // assume that the stream is large enough to hold our data
                        memcpy( p_stream1,
                                &(buf[VITA_PAYLOAD_OFFSET]),
                                VITA_NUM_SAMPLES*sizeof(uint32_t) );
                        received_stream[0] = true;
                    }
                    else if( (pkt.stream_id == (d_base_id+1)) &&
                             (received_stream[1] == false) )
                    {
                        // assume that the stream is large enough to hold our data
                        memcpy( p_stream2,
                                &(buf[VITA_PAYLOAD_OFFSET]),
                                VITA_NUM_SAMPLES*sizeof(uint32_t) );
                        received_stream[1] = true;
                    }
                }
                else
                {
                    // TODO: handle context packets and send stream notifications
                }
            } // parse packet success
            else
            {
                throw std::runtime_error("unable to parse received VITA packet");
            }
        } // receive success
    } // while receiving streams
}

bool
qtiq_vrt::parse_vrt_packet( uint32_t *p_raw_pkt, vrt_packet_t *p_pkt )
{
    bool status=true;
    uint32_t data;
    uint32_t tmp;
    uint8_t misc;


    /////////////////////////////////////
    // populate the header
    data = ntohl(p_raw_pkt[VITA_HEADER_OFFSET]);

    // parse the type
    tmp=BF_GET(data, VITA_HEADER_PKT_TYPE_OFFSET, VITA_HEADER_PKT_TYPE_LEN);
    switch( tmp )
    {
        case vrt_pkt_type_data:
            p_pkt->header.type = vrt_pkt_type_data;
            misc = VITA_HEADER_MISC_DATA_CONST;
            break;

        case vrt_pkt_type_context:
            p_pkt->header.type = vrt_pkt_type_context;
            misc = VITA_HEADER_MISC_CTXT_CONST;
            break;

        default:
            throw std::runtime_error("Received unknown packet type");
            status=false;
            break;
    }

    // make sure the MISC is ok
    tmp=BF_GET(data, VITA_HEADER_MISC_OFFSET, VITA_HEADER_MISC_LEN);
    if( tmp != misc )
    {
        throw std::runtime_error("Received malformed packet");
        status=false;
    }

    // grab the packet size
    tmp=BF_GET(data, VITA_HEADER_PKT_COUNT_OFFSET, VITA_HEADER_PKT_COUNT_LEN);
    p_pkt->header.pkt_count = tmp;

    // grab the packet count
    tmp=BF_GET(data, VITA_HEADER_PKT_SIZE_OFFSET, VITA_HEADER_PKT_SIZE_LEN);
    p_pkt->header.pkt_size = tmp;
    /////////////////////////////////////

    /////////////////////////////////////
    // stream ID
    p_pkt->stream_id = ntohl(p_raw_pkt[VITA_STREAM_OFFSET]);
    /////////////////////////////////////

    return (status);
}

}  /* namespace quadratiq */
} /* namespace gr */
