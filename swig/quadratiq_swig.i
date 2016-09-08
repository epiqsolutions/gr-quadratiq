/* -*- c++ -*- */

#define QUADRATIQ_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "quadratiq_swig_doc.i"

%{
#include "quadratiq/qtiq_source_s.h"
%}


%include "quadratiq/qtiq_source_s.h"
GR_SWIG_BLOCK_MAGIC2(quadratiq, qtiq_source_s);
