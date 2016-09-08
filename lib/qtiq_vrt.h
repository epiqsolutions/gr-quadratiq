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

#ifndef INCLUDED_QUADRATIQ_QTIQ_VRT_H
#define INCLUDED_QUADRATIQ_QTIQ_VRT_H

#include <stdint.h>
#include <cstring>
#include <string>

namespace gr {
  namespace quadratiq {

      class qtiq_vrt
      {
      private:
          int32_t d_vrt_fd;
          uint32_t d_base_id;
          uint8_t d_pkt_count;

          bool create_socket( const char *p_vrt_ip, uint32_t vrt_port, uint32_t dest_port );
          // return true if valid packet
          bool parse_vrt_packet( uint32_t *p_raw_pkt, struct vrt_packet_t *p_pkt );

      public:
          qtiq_vrt(std::string vrt_ip, uint32_t vrt_port, uint32_t dest_port, uint32_t base_stream_id);
          ~qtiq_vrt();

          void receive_data_packet( int16_t *p_stream1 );
      };
  } // namespace quadratiq
} // namespace gr

#endif /* INCLUDED_QUADRATIQ_QTIQ_VRT_H */
