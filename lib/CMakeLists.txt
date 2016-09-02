# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND quadratiq_sources
    qdiq_source_s_impl.cc
)

set(quadratiq_sources "${quadratiq_sources}" PARENT_SCOPE)
if(NOT quadratiq_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT quadratiq_sources)

add_library(gnuradio-quadratiq SHARED ${quadratiq_sources})
target_link_libraries(gnuradio-quadratiq ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-quadratiq PROPERTIES DEFINE_SYMBOL "gnuradio_quadratiq_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-quadratiq PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-quadratiq RUNTIME_COMPONENT "quadratiq_runtime" DEVEL_COMPONENT "quadratiq_devel")


########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

