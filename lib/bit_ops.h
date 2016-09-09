#ifndef BIT_OPS_H
#define BIT_OPS_H

/*! \file bit_ops.h
 * \brief This file contains a collection of macros for simplifying
 * bit manipulations.
 *
 * <pre>
 * Copyright 2011 Epiq Solutions, All Rights Reserved
 *
 *
 *</pre>*/

/***** INCLUDE *****/
#include <stdint.h>

/***** DEFINES *****/
/* The following defines are used to specify bit positions in registers */
#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7
#define BIT8 8
#define BIT9 9
#define BIT10 10
#define BIT11 11
#define BIT12 12
#define BIT13 13
#define BIT14 14
#define BIT15 15
#define BIT16 16
#define BIT17 17
#define BIT18 18
#define BIT19 19
#define BIT20 20
#define BIT21 21
#define BIT22 22
#define BIT23 23
#define BIT24 24
#define BIT25 25
#define BIT26 26
#define BIT27 27
#define BIT28 28
#define BIT29 29
#define BIT30 30
#define BIT31 31
#define BIT20 20
#define BIT21 21
#define BIT22 22
#define BIT23 23
#define BIT24 24
#define BIT25 25
#define BIT26 26
#define BIT27 27
#define BIT28 28
#define BIT29 29
#define BIT30 30
#define BIT31 31
#define BIT32 32
#define BIT33 33
#define BIT34 34
#define BIT35 35
#define BIT36 36
#define BIT37 37
#define BIT38 38
#define BIT39 39
#define BIT40 40
#define BIT41 41
#define BIT42 42
#define BIT43 43
#define BIT44 44
#define BIT45 45
#define BIT46 46
#define BIT47 47
#define BIT48 48
#define BIT49 49
#define BIT50 50
#define BIT51 51
#define BIT52 52
#define BIT53 53
#define BIT54 54
#define BIT55 55
#define BIT56 56
#define BIT57 57
#define BIT58 58
#define BIT59 59
#define BIT60 60
#define BIT61 61
#define BIT62 62
#define BIT63 63
#define BIT64 64

/* Below are helper macros for the BF_SET and BF_GET macros */
#define BIT(n)                  ( 1<<(n) )
#define BIT_SET(y, mask)        ( y |=  (mask) )
#define BIT_CLEAR(y, mask)      ( y &= ~(mask) )
#define BIT_FLIP(y, mask)       ( y ^=  (mask) )

/*! Create a bitmask of length \a len. */
#define BIT_MASK(len)           ( BIT(len)-1 )

/*! Create a bitfield mask of length \a starting at bit \a start. */
#define BF_MASK(start, len)     ( BIT_MASK(len)<<(start) )

/* ! Prepare a bitmask for insertion or combining. */
#define BF_PREP(x, start, len)  ( ((x)&BIT_MASK(len)) << (start) )

/*--------------------------------------------------------------*/
/* The following macros are the primary interface for bitfield
   manipulation */

/*! Extract a bitfield of length \a len starting at bit \a start from \a y. */
#define BF_GET(y, start, len)   ( ((y)>>(start)) & BIT_MASK(len) )

/* ! Insert a new bitfield value \a x into \a y, where y starts at
   \a start and is \a len bits long. */
#define BF_SET(y, x, start, len)    \
    ( y= ((y) &~ BF_MASK(start, len)) | BF_PREP(x, start, len) )

#endif
