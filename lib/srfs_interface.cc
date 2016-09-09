/* -*- c++ -*- */
/* 
 * Copyright 2013 Epiq Solutions.
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

#include "srfs_interface.h"

#include <math.h>
#include <string.h>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

namespace srfs {

    template <class T> bool check_range( T *p_param, T min, T max, T val );
    template <class T> bool check_resolution( T *p_param, T res, T val );

    const int SRFS_MSG_SIZE=1000;
    
    void
    update_param( srfs_param_t *p_param, const char *p_value )
    {
	if( p_value != NULL ) {
	    switch( p_param->data_type ) {
		case SRFS_UINT64:
		    sscanf( p_value, "%lu", 
			    (uint64_t*)(p_param->p_value) );
		    break;
		    
		case SRFS_UINT32:
		    sscanf( p_value, "%u", 
			    (uint32_t*)(p_param->p_value) );
		    break;
		    
		case SRFS_UINT16:
		    sscanf( p_value, "%hu", 
			    (uint16_t*)(p_param->p_value) );
		    break;
		    
		case SRFS_UINT8:
		    sscanf( p_value, "%hhu", 
			    (uint8_t*)(p_param->p_value) );
		    break;
		    
		case SRFS_FLOAT:
		    sscanf( p_value, "%f", 
			    (float*)(p_param->p_value) );
		    break;
		    
		case SRFS_ENUM:
		    *((uint32_t*)(p_param->p_value)) = 
			convert_str_to_enum( p_value,
					     p_param->p_strings, 
					     p_param->max_value );
		    break;

                case SRFS_UINT32_ACTUAL:
                    sscanf( p_value, "%u",
                            (uint32_t*)(p_param->p_value) );
                    break;
		
		default:
		    throw std::invalid_argument("invalid data_type on update");
		    break;
	    }
	}
    }

    template <class T>
    bool 
    set_param( T *p_param, T min, T max, T res, T val )
    {
	bool b_valid = false;

	b_valid = check_range( p_param, min, max, val );
	b_valid = check_resolution( p_param, res, val );

	if( b_valid )
	{
	    *p_param = val;
	}

	return b_valid;
    }

    template <class T>
    bool 
    check_range( T *p_param, T min, T max, T val )
    {
	bool b_valid = false;

	if( (val >= min) && (val <= max) )
	{
	    b_valid = true;
	}
	else
	{
	    throw std::out_of_range("value exceeds min or max");
	}
	return b_valid;
    }

    template <class T>
    bool 
    check_resolution( T *p_param, T res, T val )
    {
	bool b_valid = false;

	if( (val % res) == 0 )
	{
	    b_valid = true;
	}
	else
	{
	    throw std::out_of_range("value exceeds resolution");
	}
	return b_valid;
    }
    
    bool
    set_param( srfs_param_t *p_param, void *p_value )
    {
	bool b_valid = false;

	switch( p_param->data_type ) {
	    case SRFS_UINT64:
		b_valid = 
		    set_param<uint64_t>( (uint64_t*)(p_param->p_value),
					 (uint64_t)(p_param->min_value),
					 (uint64_t)(p_param->max_value),
					 (uint64_t)(p_param->resolution),
					 *((uint64_t*)(p_value)) );
		break;

	    case SRFS_UINT32:
		b_valid = 
		    set_param<uint32_t>( (uint32_t*)(p_param->p_value),
					 (uint32_t)(p_param->min_value),
					 (uint32_t)(p_param->max_value),
					 (uint32_t)(p_param->resolution),
					 *((uint32_t*)(p_value)) );
		break;
		
	    case SRFS_UINT16:
		b_valid = 
		    set_param<uint16_t>( (uint16_t*)(p_param->p_value),
					  (uint16_t)(p_param->min_value),
					  (uint16_t)(p_param->max_value),
					  (uint16_t)(p_param->resolution),
					 *((uint16_t*)(p_value)) );
		break;
		
	    case SRFS_UINT8:
		b_valid = 
		    set_param<uint8_t>( (uint8_t*)(p_param->p_value),
					 (uint8_t)(p_param->min_value),
					 (uint8_t)(p_param->max_value),
					 (uint8_t)(p_param->resolution),
					 *((uint8_t*)(p_value)) );
		break;
		
	    case SRFS_FLOAT:
		b_valid = 
		    check_range( (float*)(p_param->p_value),
				 (float)(p_param->min_value),
				 (float)(p_param->max_value),
				 *((float*)(p_value)) );
		/* check the resolution manually */
		if( b_valid == true )
		{
		    if( fmod( *(float*)(p_param->p_value),
			      (float)(p_param->resolution) ) == 0 )
		    {
			*((float*)(p_param->p_value)) = *((float*)(p_value));
		    }
		    else
		    {
			char e[200];
			b_valid = false;
			throw std::out_of_range("value exceeds resolution");
		    }
		}
		break;
		
	    case SRFS_ENUM:
		// just treat as uint32 since we defined our ranges appropriately already
		b_valid = 
		    set_param<uint32_t>( (uint32_t*)(p_param->p_value),
					 (uint32_t)(p_param->min_value),
					 (uint32_t)(p_param->max_value),
					 (uint32_t)(p_param->resolution),
					 *((uint32_t*)(p_value)) );
		break;
                
            case SRFS_UINT32_ACTUAL:
                // this is only a returned value, so don't allow it to be set
                b_valid = false;
                break;

	    default:
		throw std::invalid_argument("invalid data_type on set");
		break;
	}
	return b_valid;
    }
    
    uint32_t
    convert_str_to_enum( const char* pString, 
			 const std::string *pStrings, 
			 uint32_t invalid_index )
    {
	uint32_t index = invalid_index;
	
	for( index=0; index<invalid_index; index++ ) {
	    if( strncmp(pString, 
			pStrings[index].c_str(),
			strlen(pStrings[index].c_str())) == 0 ) {
		break;
	    }
	}
	if( invalid_index == index ) {
	    throw std::out_of_range("out_of_range, string not in enum");
	}
	return index;
    }

    srfs_cmd::srfs_cmd( const char *p_ip, uint32_t port, const char *p_hw_name )
    {
        char buf[SRFS_MSG_SIZE];
        char *p_str;
        d_config_port =0;
        
        create_socket( p_ip, port );

        snprintf(buf, SRFS_MSG_SIZE, "%s", "subscriptions?\r\n");
        send_msg( buf, SRFS_MSG_SIZE );
        printf("message sent, trying to receive response\r\n");
        receive_msg( buf, SRFS_MSG_SIZE );
        printf("received initial message %s\r\n", buf);

        // pull out the port #
        p_str = strtok( buf, " :" );
        while( p_str != NULL )
        {
            if( strncmp(p_str, "none", 20) == 0 )
            {
                printf("no subscriptions\r\n");
                d_config_port = 7738;
            }
            else if( strncmp(p_str, "QUADRATIQ-RX", 20) == 0 )
            {
                p_str = strtok( NULL, " :");
                printf("port is %s\r\n", p_str);
                d_config_port = atoi( p_str );
                break;
            }
            p_str = strtok( NULL, " :");
        }

        printf("srfs msg success!\r\n");
    }

    srfs_cmd::~srfs_cmd()
    {
    }

    bool
    srfs_cmd::create_socket( const char *p_ip, uint32_t port )
    {
        bool success=false;
        struct sockaddr_in sock;
        char buf[1000];

        if( (d_cmd_fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        {
            throw std::runtime_error("unable to create SRFS cmd socket");
            return false;
        }

        sock.sin_port = htons(port);
        inet_aton( p_ip, &(sock.sin_addr) );
        sock.sin_family = AF_INET;
        
        if( connect( d_cmd_fd, (struct sockaddr*)(&sock), sizeof(sock) ) != 0 )
        {
            throw std::runtime_error("unable to connect SRFS cmd socket");
            return false;
        }

        return (success);
    }

    void
    srfs_cmd::send_msg( char * msg, int length )
    {
        // make sure the buffer is null terminated
        msg[length-1] = '\0';
        // send the message
        if( send(d_cmd_fd, msg, strlen(msg), 0) != 0 )
        {
            #if 0
            printf("send fail %s, error %s\r\n", msg, strerror(errno));
            throw std::runtime_error("unable to send SRFS cmd message");
            #endif
        }
    }

    void
    srfs_cmd::receive_msg( char *response, int length )
    {
        if( recv( d_cmd_fd, response, length, 0) < 0 )
        {
            throw std::runtime_error("unable to receive SRFS response");
        }
    }
    
} // namespace srfs


