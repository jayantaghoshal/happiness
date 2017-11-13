/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <bitset>
#include <climits>

// Depending on the type T, the flexray size of the integer (in number of bits) and if the value (raw)
// is negative then this function will fill all msb bits in raw starting from bit#flexrayIntSizeInBits
template <typename T>
T bitFillValue(unsigned int flexrayIntSizeInBits, T raw) {
    // Is the value in raw negative according to the flexray integer
    // It is NOT enough to just do something like "raw<0" or similar since:
    // 1) T might be unsigned, or
    // 2) the size of the "real" value from flexray in raw does not fully fill the type T (and therfor the need to use
    // flexrayIntSizeInBits)
    // Create a bitset with the same number of bits as in "T" and initialized with the value/bits of "raw"
    const std::bitset<sizeof(T) * CHAR_BIT> set(raw);

    // flexrayIntSizeInBits is guaranteed not to be zero and also not > bits in T
    // by the static_asserts in toSignedFromRaw()
    const unsigned int signBit = flexrayIntSizeInBits - 1;
    if (set.test(signBit))  // is this value negative?
    {
        // Create a mask that for example looks like:
        // 11100000 (when T is uint8_t/int8_t and flexrayIntSizeInBits=5)
        const T mask = 0xFFFFFFFFFFFFFFFFLL << flexrayIntSizeInBits;
        // Merge mask with raw (with bitwise OR). Raw only occupies the first 5 (flexrayIntSizeInBits) bits
        // This will end up with a usefull signed, negative value that completely fills the type T
        return mask | raw;
    } else {
        return raw;  // since it is a positive number just return as it is
    }
}

template <unsigned int flexrayIntSizeInBits, typename T>
typename std::make_signed<T>::type toSignedFromRaw(T raw) {
    static_assert(flexrayIntSizeInBits != 0, "flexrayIntSizeInBits==0 for signed type is not allowed!");
    static_assert(flexrayIntSizeInBits <= sizeof(T) * CHAR_BIT, "flexrayIntSizeInBits must be <= size of T");

    return static_cast<typename std::make_signed<T>::type>(bitFillValue(flexrayIntSizeInBits, raw));
}

template <unsigned int flexrayIntSizeInBits, typename T>
typename std::make_unsigned<T>::type toUnsignedFromRaw(T raw) {
    return static_cast<typename std::make_unsigned<T>::type>(raw);
}
