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

#define NUM_RECV_ATTEMPTS (3)

#define FREQUENCY_MIN  47000000ULL
#define FREQUENCY_MAX 6000000000ULL
#define FREQUENCY_RESOLUTION 1

#define SAMPLE_RATE_MIN   233000
#define SAMPLE_RATE_MAX 61440000
#define SAMPLE_RATE_RESOLUTION 1

#define BANDWIDTH_MIN   (SAMPLE_RATE_MIN)
#define BANDWIDTH_MAX   (SAMPLE_RATE_MAX)
#define BANDWIDTH_RESOLUTION 1

#define RX_GAIN_MIN        0
#define RX_GAIN_MAX       76
#define RX_GAIN_RESOLUTION 1
            
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

        m_p_ctrl = new srfs::srfs_cmd( ctrl_ip.c_str(), ctrl_port, "QUADRATIQ-RX" );
        // initialize the parameters
        init_srfs_params();
        // initialize the default parameters
        d_freqA = FREQUENCY_MIN;
        d_freqB = FREQUENCY_MIN;
        d_sampleRateA = SAMPLE_RATE_MIN;
        d_sampleRateB = SAMPLE_RATE_MIN;
        d_bandwidthA = BANDWIDTH_MIN;
        d_bandwidthB = BANDWIDTH_MIN;

        d_gainA1 = RX_GAIN_MAX;
        d_gainA2 = RX_GAIN_MAX;
        d_gainB1 = RX_GAIN_MAX;
        d_gainB2 = RX_GAIN_MAX;

    }

    /*
     * Our virtual destructor.
     */
    qtiq_source_s_impl::~qtiq_source_s_impl()
    {
        delete m_p_chipA;
        delete m_p_chipB;
        delete m_p_ctrl;
    }

    uint64_t
    qtiq_source_s_impl::set_center_freqA(uint64_t freq)
    {
        m_p_ctrl->set_param( "A1:frequency", &freq);
        return (d_freqA);
    }
      
    uint64_t
    qtiq_source_s_impl::center_freqA(void)
    {
        return (d_freqA);
    }

    uint64_t
    qtiq_source_s_impl::set_center_freqB(uint64_t freq)
    {
        m_p_ctrl->set_param( "B1:frequency", &freq);
        return (d_freqB);
    }
      
    uint64_t
    qtiq_source_s_impl::center_freqB(void)
    {
        return (d_freqB);
    }

    uint32_t
    qtiq_source_s_impl::set_sample_rateA(uint32_t sample_rate)
    {
        m_p_ctrl->set_param( "A1:sample_rate", &sample_rate);
        return (d_sampleRateA);
    }
    uint32_t
    qtiq_source_s_impl::sample_rateA(void)
    {
        return (d_sampleRateA);
    }

      uint32_t
    qtiq_source_s_impl::set_sample_rateB(uint32_t sample_rate)
    {
        m_p_ctrl->set_param( "B1:sample_rate", &sample_rate);
        return (d_sampleRateB);
    }
    uint32_t
    qtiq_source_s_impl::sample_rateB(void)
    {
        return (d_sampleRateB);
    }

    uint32_t
    qtiq_source_s_impl::set_bandwidthA(uint32_t bandwidth)
    {
        m_p_ctrl->set_param( "A1:bandwidth", &bandwidth );
        return (d_bandwidthA);
    }
    uint32_t
    qtiq_source_s_impl::bandwidthA(void)
    {
        return (d_bandwidthA);
    }

    uint32_t
    qtiq_source_s_impl::set_bandwidthB(uint32_t bandwidth)
    {
        m_p_ctrl->set_param( "B1:bandwidth", &bandwidth );
        return (d_bandwidthA);
    }
    uint32_t
    qtiq_source_s_impl::bandwidthB(void)
    {
        return (d_bandwidthB);
    }
      
    uint8_t
    qtiq_source_s_impl::set_gainA1(uint8_t gain)
    {
        m_p_ctrl->set_param("A1:rx_gain", &gain);
        return (d_gainA1);
    }
    uint8_t
    qtiq_source_s_impl::gainA1()
    {
        return (d_gainA1);
    }

    uint8_t
    qtiq_source_s_impl::set_gainA2(uint8_t gain)
    {
        m_p_ctrl->set_param("A2:rx_gain", &gain);
        return (d_gainA2);
    }
    uint8_t
    qtiq_source_s_impl::gainA2()
    {
        return (d_gainA2);
    }

    uint8_t
    qtiq_source_s_impl::set_gainB1(uint8_t gain)
    {
        m_p_ctrl->set_param("B1:rx_gain", &gain);
        return (d_gainB1);
    }
    uint8_t
    qtiq_source_s_impl::gainB1()
    {
        return (d_gainB1);
    }

    uint8_t
    qtiq_source_s_impl::set_gainB2(uint8_t gain)
    {
        m_p_ctrl->set_param("B2:rx_gain", &gain);
        return (d_gainB2);
    }
    uint8_t
    qtiq_source_s_impl::gainB2()
    {
        return (d_gainB2);
    }

      
    void
    qtiq_source_s_impl::init_srfs_params(void)
    {
        // frequency
        m_p_ctrl->add_param( "A1:frequency",
                             srfs::SRFS_UINT64,
                             (void*)(&d_freqA),
                             FREQUENCY_MIN,
                             FREQUENCY_MAX,
                             FREQUENCY_RESOLUTION,
                             NULL );
        // frequency
        m_p_ctrl->add_param( "B1:frequency",
                             srfs::SRFS_UINT64,
                             (void*)(&d_freqB),
                             FREQUENCY_MIN,
                             FREQUENCY_MAX,
                             FREQUENCY_RESOLUTION,
                             NULL );

        // sample rate
        m_p_ctrl->add_param( "A1:sample_rate",
                             srfs::SRFS_UINT32,
                             (void*)(&d_sampleRateA),
                             SAMPLE_RATE_MIN,
                             SAMPLE_RATE_MAX,
                             SAMPLE_RATE_RESOLUTION,
                             NULL );
        // sample rate
        m_p_ctrl->add_param( "B1:sample_rate",
                             srfs::SRFS_UINT32,
                             (void*)(&d_sampleRateB),
                             SAMPLE_RATE_MIN,
                             SAMPLE_RATE_MAX,
                             SAMPLE_RATE_RESOLUTION,
                             NULL );
        
        // bandwidth
        m_p_ctrl->add_param( "A1:bandwidth",
                             srfs::SRFS_UINT32,
                             (void*)(&d_bandwidthA),
                             BANDWIDTH_MIN,
                             BANDWIDTH_MAX,
                             BANDWIDTH_RESOLUTION,
                             NULL );
        // bandwidth
        m_p_ctrl->add_param( "B1:bandwidth",
                             srfs::SRFS_UINT32,
                             (void*)(&d_bandwidthB),
                             BANDWIDTH_MIN,
                             BANDWIDTH_MAX,
                             BANDWIDTH_RESOLUTION,
                             NULL );

        // rx gain
        m_p_ctrl->add_param( "A1:rx_gain",
                             srfs::SRFS_UINT8,
                             (void*)(&d_gainA1),
                             RX_GAIN_MIN,
                             RX_GAIN_MAX,
                             RX_GAIN_RESOLUTION,
                             NULL );
        m_p_ctrl->add_param( "A2:rx_gain",
                             srfs::SRFS_UINT8,
                             (void*)(&d_gainA2),
                             RX_GAIN_MIN,
                             RX_GAIN_MAX,
                             RX_GAIN_RESOLUTION,
                             NULL );
        m_p_ctrl->add_param( "B1:rx_gain",
                             srfs::SRFS_UINT8,
                             (void*)(&d_gainB1),
                             RX_GAIN_MIN,
                             RX_GAIN_MAX,
                             RX_GAIN_RESOLUTION,
                             NULL );
        m_p_ctrl->add_param( "B2:rx_gain",
                             srfs::SRFS_UINT8,
                             (void*)(&d_gainB2),
                             RX_GAIN_MIN,
                             RX_GAIN_MAX,
                             RX_GAIN_RESOLUTION,
                             NULL );
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

