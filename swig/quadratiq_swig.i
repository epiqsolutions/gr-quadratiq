/* -*- c++ -*- */

#define QUADRATIQ_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "quadratiq_swig_doc.i"

%{
#include "quadratiq/qdiq_source_s.h"
%}


%include "quadratiq/qdiq_source_s.h"
GR_SWIG_BLOCK_MAGIC2(quadratiq, qdiq_source_s);
