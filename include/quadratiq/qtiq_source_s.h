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


#ifndef INCLUDED_QUADRATIQ_QTIQ_SOURCE_S_H
#define INCLUDED_QUADRATIQ_QTIQ_SOURCE_S_H

#include <quadratiq/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace quadratiq {

    /*!
     * \brief <+description of block+>
     * \ingroup quadratiq
     *
     */
    class QUADRATIQ_API qtiq_source_s : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<qtiq_source_s> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of quadratiq::qtiq_source_s.
       *
       * To avoid accidental use of raw pointers, quadratiq::qtiq_source_s's
       * constructor is in a private implementation
       * class. quadratiq::qtiq_source_s::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string ctrl_ip, uint32_t ctrl_port);

      /*! 
       * \brief Set center frequency for chip A with Hz resolution.
       * \param freq The frequency in Hz
       * \return the actual center frequency
       *
       * Set the center frequency of chip A of Quadratiq
       */
      virtual uint64_t set_center_freqA(uint64_t freq) = 0;
      /*! 
       * \brief Get center frequency with Hz resolution.
       * \return the actual center frequency
       *
       * Get the center frequency of chip A of Quadratiq.
       */
      virtual uint64_t center_freqA(void) = 0;

      /*! 
       * \brief Set center frequency for chip B with Hz resolution.
       * \param freq The frequency in Hz
       * \return the actual center frequency
       *
       * Set the center frequency of chip B of Quadratiq
       */
      virtual uint64_t set_center_freqB(uint64_t freq) = 0;
      /*! 
       * \brief Get center frequency with Hz resolution.
       * \return the actual center frequency
       *
       * Get the center frequency of chip B of Quadratiq.
       */
      virtual uint64_t center_freqB(void) = 0;

      /*! 
       * \brief Set the sample rate for chip A
       * \param sample_rate The sample rate
       * \return the actual sample rate
       *
       * Set the sample rate of chip A of Quadratiq. 
       */
      virtual uint32_t set_sample_rateA(uint32_t sample_rate) = 0;
      /*! 
       * \brief Get the sample rate of chip A
       * \return the actual sample rate
       *
       * Get the sample rate of the Quadratiq chip A.
       */
      virtual uint32_t sample_rateA(void) = 0;
      
      /*! 
       * \brief Set the sample rate for chip B
       * \param sample_rate The sample rate
       * \return the actual sample rate
       *
       * Set the sample rate of chip A of Quadratiq. 
       */
      virtual uint32_t set_sample_rateB(uint32_t sample_rate) = 0;
            /*! 
       * \brief Get the sample rate of chip B
       * \return the actual sample rate
       *
       * Get the sample rate of the Quadratiq chip B. 
       */
      virtual uint32_t sample_rateB(void) = 0;

      /*! 
       * \brief Set the channel bandwidth for chip A
       * \param bandwidth The channel bandwidth
       * \return the actual bandwidth
       * 
       * The requested channel bandwidth configures
       * the FIR filter coefficients.  The requested channel bandwidth 
       * does not always match the actual bandwidth, so the actual bandwidth
       * should be queried after configuring.
       */
      virtual uint32_t set_bandwidthA(uint32_t bandwidth) = 0;
      /*! 
       * \brief Get the channel bandwidth for chip A
       * \return the actual channel bandwidth
       *
       * Get the channel bandwidth of the Quadratiq. 
       */
      virtual uint32_t bandwidthA(void) = 0;

      /*! 
       * \brief Set the channel bandwidth for chip B
       * \param bandwidth The channel bandwidth
       * \return the actual bandwidth
       * 
       * The requested channel bandwidth configures
       * the FIR filter coefficients.  The requested channel bandwidth 
       * does not always match the actual bandwidth, so the actual bandwidth
       * should be queried after configuring.
       */
      virtual uint32_t set_bandwidthB(uint32_t bandwidth) = 0;
      /*! 
       * \brief Get the channel bandwidth for chip B
       * \return the actual channel bandwidth
       *
       * Get the channel bandwidth of the Quadratiq. 
       */
      virtual uint32_t bandwidthB(void) = 0;

      /*!
       * \brief Set the value of the A1 gain 
       * \param gain rx gain value (0-76 dB)
       * \return actual rx gain
       *
       * Set the value of the Rx gain
       */
      virtual uint8_t set_gainA1(uint8_t gain) = 0;
       /*!
	* \brief Get the value of the A1 gain 
	* \return actual rx gain
	*
	* Get the value of the Rx gain
	*/
       virtual uint8_t gainA1(void) = 0;

       /*!
       * \brief Set the value of the A2 gain 
       * \param gain rx gain value (0-76 dB)
       * \return actual rx gain
       *
       * Set the value of the Rx gain
       */
      virtual uint8_t set_gainA2(uint8_t gain) = 0;
       /*!
	* \brief Get the value of the A2 gain 
	* \return actual rx gain
	*
	* Get the value of the Rx gain
	*/
       virtual uint8_t gainA2(void) = 0;

       /*!
       * \brief Set the value of the B1 gain 
       * \param gain rx gain value (0-76 dB)
       * \return actual rx gain
       *
       * Set the value of the Rx gain
       */
      virtual uint8_t set_gainB1(uint8_t gain) = 0;
       /*!
	* \brief Get the value of the B1 gain 
	* \return actual rx gain
	*
	* Get the value of the Rx gain
	*/
       virtual uint8_t gainB1(void) = 0;

       /*!
       * \brief Set the value of the B2 gain 
       * \param gain rx gain value (0-76 dB)
       * \return actual rx gain
       *
       * Set the value of the Rx gain
       */
      virtual uint8_t set_gainB2(uint8_t gain) = 0;
       /*!
	* \brief Get the value of the B2 gain 
	* \return actual rx gain
	*
	* Get the value of the Rx gain
	*/
       virtual uint8_t gainB2(void) = 0;
    };

  } // namespace quadratiq
} // namespace gr

#endif /* INCLUDED_QUADRATIQ_QTIQ_SOURCE_S_H */

