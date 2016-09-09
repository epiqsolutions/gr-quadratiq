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

#ifndef INCLUDED_QUADRATIQ_QTIQ_SOURCE_S_IMPL_H
#define INCLUDED_QUADRATIQ_QTIQ_SOURCE_S_IMPL_H

#include <quadratiq/qtiq_source_s.h>

#include <qtiq_vrt.h>
#include <srfs_interface.h>

namespace gr {
  namespace quadratiq {

    class qtiq_source_s_impl : public qtiq_source_s
    {
     private:
        qtiq_vrt *m_p_chipA;
        qtiq_vrt *m_p_chipB;

        srfs::srfs_cmd *m_p_ctrl;

        uint64_t d_freqA;
        uint64_t d_freqB;

        uint32_t d_sampleRateA;
        uint32_t d_sampleRateB;

        uint32_t d_bandwidthA;
        uint32_t d_bandwidthB;

        uint32_t d_actualBandwidthA;
        uint32_t d_actualBandwidthB;

        // initializes params map
        void init_srfs_params(void);

        // configures parameter specified by token to the value provided
        void set_param( const std::string token, void *pValue );

     public:
      qtiq_source_s_impl(std::string ctrl_ip, uint32_t ctrl_port);
      ~qtiq_source_s_impl();

      // configuration functions
      uint64_t set_center_freqA(uint64_t freq);
      uint64_t center_freqA(void);

      uint64_t set_center_freqB(uint64_t freq);
      uint64_t center_freqB(void);

      uint32_t set_sample_rateA(uint32_t sample_rate);
      uint32_t sample_rateA();  

      uint32_t set_sample_rateB(uint32_t sample_rate);
      uint32_t sample_rateB();  

      uint32_t set_bandwidthA(uint32_t bandwidth);
      uint32_t bandwidthA();  

      uint32_t set_bandwidthB(uint32_t bandwidth);
      uint32_t bandwidthB();  

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

        static const uint32_t DEFAULT_BASE_ID=1935998976;
    };

  } // namespace quadratiq
} // namespace gr

#endif /* INCLUDED_QUADRATIQ_QTIQ_SOURCE_S_IMPL_H */

