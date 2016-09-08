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

void
qtiq_vrt::receive_data_packet( uint32_t *p_stream1, uint32_t *p_stream2 )
{
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

}  /* namespace quadratiq */
} /* namespace gr */
