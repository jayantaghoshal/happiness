/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#if defined(WIN32)

#include <limits.h>
#include <stddef.h>
typedef int ssize_t;

typedef char char_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int int16_t;
typedef unsigned short int uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

typedef int8_t int_least8_t;
typedef uint8_t uint_fast8_t;
typedef int bool_fast;

#define UINT8_MIN (0)
#define UINT8_MAX _UI8_MAX
#define UINT16_MIN (0)
#define UINT16_MAX _UI16_MAX
#define UINT32_MIN (0)
#define UINT32_MAX _UI32_MAX

#define INT8_MIN _I8_MIN
#define INT8_MAX _I8_MAX
#define INT16_MIN _I16_MIN
#define INT16_MAX _I16_MAX
#define INT32_MIN _I32_MIN
#define INT32_MAX _I32_MAX
#else

#include <inttypes.h>
typedef char char_t;

#endif /* WIN32 */

typedef uint8_t bool_t;

// typedef enum                                              /* boolean type */
/*    {
    False_v = (0 == 1),
    True_v  = (0 == 0)
    }
    bool_t;
*/

/**
 * Compute the number of elements in an array
 * @param array   name of array
 * @return number of array elements
 */
/* PRQA S 3410 1 */ /* Silence QAC warning about no parentheses around "array" */
#define Num_Elems(array) (sizeof(array) / sizeof(array[0]))

#ifndef __cplusplus

/* The 'bool' type is used by the XSAL
 */
typedef uint8_t bool;
enum bool_Tag { false = 0, true = 1 };

#endif /* __cplusplus */
