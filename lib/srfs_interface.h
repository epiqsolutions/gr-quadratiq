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

#ifndef SRFS_INTERFACE_H
#define SRFS_INTERFACE_H

#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>

#include <map>

/** This file contains utilities to assist with communicating
    with the SRFS app */

namespace srfs {

    /** parameter data types */
    typedef enum 
    {
	SRFS_UINT64,
	SRFS_UINT32,
	SRFS_UINT16,
	SRFS_UINT8,
	SRFS_FLOAT,
	SRFS_ENUM,
        SRFS_UINT32_ACTUAL,
    } SRFS_DATATYPES;
    
    /** container of a SRFS parameter */
    typedef struct 
    {
	SRFS_DATATYPES data_type;     /* type of data */
	void *p_value;                /* pointer to actual value of parameter */
	int64_t min_value;            /* min value of parameter */
	int64_t max_value;            /* max value of parameter */
	float resolution;             /* min resolution of parameter */
	const std::string *p_strings; /* array of strings (used for enums only) */
    } srfs_param_t;
    
    /* validates the value provided to ensure that it it falls within
       the range and resolution.  If it's valid, then it sets the p_value */
    bool set_param( srfs_param_t *p_param, void* p_value );
    
    /* reads in the string representation of the value and stores it
       in p_param */
    void update_param( srfs_param_t *p_param, const char* p_value );

    /* looks through *pStrings array for a match to pString.  invalid_index
       is the max number of elements contained on pStrings */
    uint32_t convert_str_to_enum( const char* pString, 
				  const std::string *pStrings, 
				  uint32_t invalid_index );

    class srfs_cmd
    {
    private:
        int32_t d_cmd_fd;
        uint32_t d_config_port;

        uint32_t d_rx_freqA;
        uint32_t d_rx_freqB;

        std::string m_str_hardware_block;

        // string to srfs_param_t map
        typedef std::map<const std::string, srfs::srfs_param_t> param_map;
        // pair string to srfs_param_t
        typedef std::pair<const std::string, srfs::srfs_param_t> param_pair;
        // map of valid parameters and associated strings
        param_map m_params;

        void send_config();
        void send_msg( char* msg, int length );
        void receive_msg( char *response, int length );

        bool create_socket( const char *p_ip, uint32_t port );

    public:
        srfs_cmd( const char *p_ip, uint32_t port, const char *p_hw_name );
        ~srfs_cmd();

        // adds a parameter to the maveriq_params map
        void add_param( const std::string token,
                        srfs::SRFS_DATATYPES data_type,
                        void *p_value,
                        int64_t min_value,
                        int64_t max_value,
                        float resolution,
                        const std::string *p_strings );

        // configures parameter specified by token to the value provided
        void set_param( const std::string token, void *pValue );

    };
} // namespace SRFS

#endif
