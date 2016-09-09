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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "qtiq_source_s_impl.h"

namespace gr {
  namespace quadratiq {
            
    qtiq_source_s::sptr
    qtiq_source_s::make(std::string ctrl_ip, uint32_t ctrl_port)
    {
      return gnuradio::get_initial_sptr
        (new qtiq_source_s_impl(ctrl_ip, ctrl_port));
    }

    /*
     * The private constructor
     */
    qtiq_source_s_impl::qtiq_source_s_impl(std::string ctrl_ip, uint32_t ctrl_port)
      : gr::sync_block("qtiq_source_s",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(4, 4, sizeof(short)))
    {
        set_output_multiple(qtiq_vrt::VITA_NUM_SAMPLES*2);
        
        // create the VRT stream
        m_p_chipA = new qtiq_vrt( "192.168.5.2", 8000, 9880, DEFAULT_BASE_ID );
        m_p_chipB = new qtiq_vrt( "192.168.4.6", 8000, 9879, DEFAULT_BASE_ID+2 );
    }

    /*
     * Our virtual destructor.
     */
    qtiq_source_s_impl::~qtiq_source_s_impl()
    {
        delete m_p_chipA;
        delete m_p_chipB;
    }

    int
    qtiq_source_s_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      short *out1 = (short *) output_items[0];
      short *out2 = (short *) output_items[1];
      short *out3 = (short *) output_items[2];
      short *out4 = (short *) output_items[3];

      // receive a data packet
      m_p_chipA->receive_data_packet( out1, out2 );
      m_p_chipB->receive_data_packet( out3, out4 );
      
      // TODO: actually parse # samples
      noutput_items = (qtiq_vrt::VITA_NUM_SAMPLES)*2; 
      
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace quadratiq */
} /* namespace gr */

