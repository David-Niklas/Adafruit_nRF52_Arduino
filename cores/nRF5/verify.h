/******************************************************************************/
/*!
    @file     verify.h
    @author   hathach (tinyusb.org)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
    Copyright (c) 2018, Adafruit Industries (adafruit.com)
    Copyright (c) 2016, hathach (tinyusb.org)

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/******************************************************************************/

#ifndef _VERIFY_H_
#define _VERIFY_H_

#include "compiler_macro.h"

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------+
// Compile-time Assert
//--------------------------------------------------------------------+
#if defined __COUNTER__ && __COUNTER__ != __COUNTER__
  #define _VERIFY_COUNTER __COUNTER__
#else
  #define _VERIFY_COUNTER __LINE__
#endif

#define VERIFY_STATIC(const_expr) enum { XSTRING_CONCAT_(static_verify_, _VERIFY_COUNTER) = 1/(!!(const_expr)) }

//--------------------------------------------------------------------+
// VERIFY Helper
//--------------------------------------------------------------------+
#if CFG_DEBUG >= 1
  #define VERIFY_MESS(_status, _funcstr) \
    do { \
      const char* (*_fstr)(int32_t) = _funcstr;\
      printf("%s: %d: verify failed, error = ", __PRETTY_FUNCTION__, __LINE__);\
      if (_fstr) printf(_fstr(_status)); else printf("%d", _status);\
      printf("\n");\
    }while(0)
#else
  #define VERIFY_MESS(_status, _funcstr)
#endif


#define VERIFY_ERR_DEF(_status, _ret, _funcstr) \
    if ( 0 != _status ) {                       \
      VERIFY_MESS(_status, _funcstr);           \
      return _ret;                              \
    }

// Helper to implement optional parameter for VERIFY Macro family
#define _GET_3RD_ARG(arg1, arg2, arg3, ...)  arg3

/*------------------------------------------------------------------*/
/* VERIFY STATUS
 * - VERIFY_ERR_1ARGS : return status of condition if failed
 * - VERIFY_ERR_2ARGS : return provided status code if failed
 *------------------------------------------------------------------*/
#define VERIFY_ERR_1ARGS(_exp, _funcstr)          \
    do {                                          \
      int32_t _status = (int32_t)(_exp);          \
      VERIFY_ERR_DEF(_status, _status, _funcstr); \
    } while(0)

#define VERIFY_ERR_2ARGS(_exp, _ret, _funcstr)    \
    do {                                          \
      int32_t _status = (int32_t)(_exp);          \
      VERIFY_ERR_DEF(_status, _ret, _funcstr);    \
    } while(0)

/**
 * Check if status is success (zero), otherwise return
 * - status value if called with 1 parameter e.g VERIFY_STATUS(status)
 * - 2 parameter if called with 2 parameters e.g VERIFY_STATUS(status, errorcode)
 */
#define VERIFY_STATUS(...)  _GET_3RD_ARG(__VA_ARGS__, VERIFY_ERR_2ARGS, VERIFY_ERR_1ARGS)(__VA_ARGS__, dbg_err_str)

#define VERIFY_ERROR(...)   _GET_3RD_ARG(__VA_ARGS__, VERIFY_ERR_2ARGS, VERIFY_ERR_1ARGS)(__VA_ARGS__, NULL)

/*------------------------------------------------------------------*/
/* VERIFY
 * - VERIFY_1ARGS : return condition if failed (false)
 * - VERIFY_2ARGS : return provided value if failed
 *------------------------------------------------------------------*/
#define VERIFY_1ARGS(cond)            if (!(cond)) return false;
#define VERIFY_2ARGS(cond, _error)    if (!(cond)) return _error;

/**
 * Check if condition is success (true), otherwise return
 * - false value if called with 1 parameter e.g VERIFY(condition)
 * - 2 parameter if called with 2 parameters e.g VERIFY(condition, errorcode)
 */
#define VERIFY(...)  _GET_3RD_ARG(__VA_ARGS__, VERIFY_2ARGS, VERIFY_1ARGS)(__VA_ARGS__)

// TODO VERIFY with final statement

#ifdef __cplusplus
}
#endif

#endif /* _VERIFY_H_ */
