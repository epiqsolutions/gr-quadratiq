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
              gr::io_signature::make(1, 1, sizeof(short)))
    {}

    /*
     * Our virtual destructor.
     */
    qtiq_source_s_impl::~qtiq_source_s_impl()
    {
    }

    int
    qtiq_source_s_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      short *out = (short *) output_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace quadratiq */
} /* namespace gr */

