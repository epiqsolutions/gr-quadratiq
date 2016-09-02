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


#ifndef INCLUDED_QUADRATIQ_QDIQ_SOURCE_S_H
#define INCLUDED_QUADRATIQ_QDIQ_SOURCE_S_H

#include <quadratiq/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace quadratiq {

    /*!
     * \brief <+description of block+>
     * \ingroup quadratiq
     *
     */
    class QUADRATIQ_API qdiq_source_s : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<qdiq_source_s> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of quadratiq::qdiq_source_s.
       *
       * To avoid accidental use of raw pointers, quadratiq::qdiq_source_s's
       * constructor is in a private implementation
       * class. quadratiq::qdiq_source_s::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string ctrl_ip, uint32_t ctrl_port);
    };

  } // namespace quadratiq
} // namespace gr

#endif /* INCLUDED_QUADRATIQ_QDIQ_SOURCE_S_H */

