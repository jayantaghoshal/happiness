/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/**
 *****************************************************************************
 * @defgroup TEM3_BASIC_TYPES_INTERFACE Basic types
 * @ingroup TEM3_APPLICATION_INTERFACE
 * @{
 *
 * Copyright (C) 2011 Actia Nordic AB. All rights reserved
 *
 * \par    Project:
 *         TEM3
 *
 * \par    Module:
 *         TEM3 Basic types
 *
 * \par    Purpose:
 *         Export basic types.
 *
 * \par    Usage Notes:
 *         None
 *
 *****************************************************************************
 */

#pragma once

#include <stdbool.h> /* For better C++ interoperability. Not in original code. */
#include <stddef.h>
#include <stdint.h>

/**
 * \par Description
 *  The general status type for TEM. Should be used by platform modules
 *  interface functions to signal their status.
 *  - \c E_TEM_OK:            Success.
 *  - \c E_TEM_ERROR:         Unspecified error.
 *  - \c E_TEM_ILLEGAL_PARAM: Illegal parameter value specified in call.
 *  - \c E_TEM_NOT_READY:     The required function can not yet be performed.
 *  - \c E_TEM_BUSY:          The required resource is currently busy.
 *  - \c E_TEM_EMPTY:         There is no data to receive from the resource.
 *  - \c E_TEM_NOT_AVAILABLE  The requested operation is not available.
 */
typedef enum {
    E_TEM_OK,
    E_TEM_ERROR,
    E_TEM_ILLEGAL_PARAM,
    E_TEM_BUSY,
    E_TEM_NOT_READY,
    E_TEM_EMPTY,
    E_TEM_NOT_AVAILABLE
} TEM_Status;

/** \defgroup TEM3_BASIC_TYPES_INTERFACE_INTS Integer types.
*** \ingroup TEM3_BASIC_TYPES_INTERFACE
*** @{
*** \par Integer types. */

#if 0 /* Disable original code */
#if defined(__ICCSH__) && (1 == __ICCSH__)
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;
typedef signed char S8;
typedef signed short S16;
typedef signed int S32;
typedef signed long long S64;
#elif defined(MSC_VER)
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef unsigned long long U64;
typedef signed char S8;
typedef signed short S16;
typedef signed long S32;
typedef signed long long S64;
#endif

#ifndef bool
#define bool unsigned char
#define true 1
#define false 0
#endif
#else
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;
typedef signed char S8;
typedef signed short S16;
typedef signed int S32;
typedef signed long long S64;
#endif

//@}

/** \defgroup TEM3_BASIC_TYPES_INTERFACE_NOINIT No initialize attribute.
*** \ingroup TEM3_BASIC_TYPES_INTERFACE
*** @{
*** \par Description
*** If a variable does not need to be initilized, handled in its init method
*** it shall be prefixed by __NO_INIT__, to ensure that the compiler excludes
*** it from the segement_init_zero/copy in the cstartup. If this is not performed
*** it will add time to the startup (will be done before the main() method is called).
***/

#if defined(__ICCSH__) && (1 == __ICCSH__)
#ifdef __NO_INIT__
#error "bad configuration"
#elif defined(_MSC_VER)
#define __NO_INIT__
#else
#define __NO_INIT__ __no_init
#endif
#endif

/**
 * Use the macro to append new line characters to a string.
 * \par Description
 * "my text that has no end of line characters" EOL
 */
#define EOL "\r\n"

//@}

/** @} */ /* End Doxygen defgroup */
