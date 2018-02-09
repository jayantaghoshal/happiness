/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/** @addtogroup PL_ASN_BASE
*** @{
***
*** Copyright (C) 2012 ACTIA NORDIC AB. All rights reserved
***
*** @par Project:
*** TEM3
***
*** @par Module:
*** SL/ASN_BASE ASN.1 Interface
***
*** @par Implementation Notes:
*** None.
***
***/

#include "pl/asn_base/asn_base.h"
#include <limits.h>
#include <string.h>

// --------------------------------------------------------------------------
// Local Definitions.

/// Default character set
#define DEFAULT_CHARACTER_SET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 '()+,-./:=?"

/// See common_utils.h
#define UNUSED_VARIABLE(x) (void)(x)

// 1 << nrbits == 2^nrbits, so if 2^nrbits < range perform indexing else do not.
#define ASN_DO_INDEXING(max, nrbits) (1 << nrbits) < max + 1 ? TRUE : FALSE

// --------------------------------------------------------------------------
// Local Type Declarations.
struct ASN_Session_t {
    ASN_BYTE* malloc_buffer;
    ASN_BYTE* malloc_next_ptr;
    ASN_BYTE* malloc_end_ptr;
    U32 malloc_buf_size;
    struct ASN_Result_t error;
};

struct ASN_Stream_t {
    /* Index into the buffer, marking the byte next to be handled. */
    U32 bytePointer;
    /* Stores how many bits in the active byte that are not yeat used or read. */
    U32 bitPointer;
    ASN_BYTE* startPtr;
    ASN_BYTE* endPtr;
    struct ASN_Result_t error;
};

struct ASN_ObjectId_t {
    U16* data;
    U16 length;
    BOOL isRelative;
};

struct ASN_String_t {
    unsigned char* data;
    U32 length;
};

struct ASN_PrintableString_t {
    unsigned char* characterSet;
    unsigned char* data;
    U32 characterSetSize;
    U32 length;
};

// --------------------------------------------------------------------------
// Local Variable Definitions.
// --------------------------------------------------------------------------
// Exported Variable Definitions.
// --------------------------------------------------------------------------
// Local Function Declarations.
ASN_Result ASN_Session_GetResult(ASN_Session session);
static void ASN_Stream_CheckAndAlign(ASN_Stream stream);
static U32 ASN_Stream_DecodeBits(ASN_Stream stream, U32 bitCount);
static ASN_BYTE ASN_Stream_DecodeByte(ASN_Stream stream);
static U32 ASN_Stream_DecodeInt(ASN_Stream stream, U32 bitCount);
static ASN_Result ASN_Stream_EncodeBits(ASN_Stream stream, ASN_BYTE data, U32 bitCount);
static ASN_Result ASN_Stream_EncodeByte(ASN_Stream stream, ASN_BYTE data);
ASN_Result ASN_Stream_EncodeInt(ASN_Stream stream, U32 value, U32 bitCount);
#ifdef ASN_ENABLE_TESTCODE
static BOOL assertEqualsU32(U32 should_be, U32 is, U32 line_number);
static BOOL assertNotEqualsPtr(void* should_not_be, void* is, U32 line_number);
static BOOL assertString(char* should_be, char* is, U32 line_number);
static int charToNibble(char c);
#endif
#ifdef ASN_PRINT
static void ASN_String_Print(struct ASN_String_t* ThisPtr);
#endif /* #ifdef ASN_PRINT */

// --------------------------------------------------------------------------
// Local Function Definitions.
static void ASN_Stream_CheckAndAlign(ASN_Stream stream) {
    if (stream->bitPointer == 0) {
        stream->bitPointer = 8;
        stream->bytePointer++;
    }
}

static U32 ASN_Stream_DecodeBits(ASN_Stream stream, U32 bitCount) {
    U32 data;
    if (bitCount <= stream->bitPointer) {
        ASN_BYTE mask = (ASN_BYTE)0xff;
        /* The Bit String fits within the current Byte. */
        data = stream->startPtr[stream->bytePointer];
        data >>= (stream->bitPointer - bitCount);
        mask <<= bitCount;
        data &= ~mask;
        stream->bitPointer -= bitCount;
        ASN_Stream_CheckAndAlign(stream);
    } else {
        U32 leftoverPart;
        U32 leftovers = bitCount - stream->bitPointer;
        data = ASN_Stream_DecodeBits(stream, stream->bitPointer);
        data <<= leftovers;
        leftoverPart = ASN_Stream_DecodeBits(stream, leftovers);
        data |= leftoverPart;
    }
    return data;
}

static ASN_BYTE ASN_Stream_DecodeByte(ASN_Stream stream) {
    if (stream->bitPointer == 8) {
        return stream->startPtr[stream->bytePointer++];
    } else {
        return (ASN_BYTE)ASN_Stream_DecodeBits(stream, 8);
    }
}

static U32 ASN_Stream_DecodeInt(ASN_Stream stream, U32 bitCount) {
    long value = 0;
    while (bitCount > 0) {
        if (bitCount < 8) {
            value = (value << bitCount) + ASN_Stream_DecodeBits(stream, bitCount);
            bitCount = 0;
        } else {
            ASN_BYTE part = ASN_Stream_DecodeByte(stream);
            value = (value << 8) + ((part & 0x80) == 0 ? (U32)part : (part & (ASN_BYTE)0x7f) + 0x80);
            bitCount -= 8;
        }
    }
    return value;
}

static ASN_Result ASN_Stream_EncodeBits(ASN_Stream stream, ASN_BYTE data, U32 bitCount) {
    ASN_BYTE mask = (ASN_BYTE)0xff;

    if (ASN_Stream_BitsLeft(stream) < bitCount) {
        ASN_Result result = &stream->error;
        result->error = ASN_ERROR_BUFFER_FULL;
        return result;
    }

    /* "New" byte to process */
    if (stream->bitPointer == 8) stream->startPtr[stream->bytePointer] = (ASN_BYTE)0;

    mask <<= bitCount;
    data &= ~mask;
    if (bitCount <= stream->bitPointer) {
        /* The Bit String fits within the last Byte. */
        data <<= (stream->bitPointer - bitCount);
        stream->startPtr[stream->bytePointer] |= data;
        stream->bitPointer -= bitCount;
        ASN_Stream_CheckAndAlign(stream);
    } else {
        U32 leftovers = bitCount - stream->bitPointer;
        ASN_BYTE firstPart = data;
        firstPart >>= (bitCount - stream->bitPointer);
        ASN_Stream_EncodeBits(stream, firstPart, stream->bitPointer);
        mask = (ASN_BYTE)0xff;
        mask <<= leftovers;
        ASN_Stream_EncodeBits(stream, (ASN_BYTE)(data & ~mask), leftovers);
    }
    return ASN_RESULT_OK;
}

static ASN_Result ASN_Stream_EncodeByte(ASN_Stream stream, ASN_BYTE data) {
    if (ASN_Stream_BitsLeft(stream) < 8) {
        ASN_Result result = &stream->error;
        result->error = ASN_ERROR_BUFFER_FULL;
        return result;
    }
    if (stream->bitPointer == 8) {
        stream->startPtr[stream->bytePointer++] = data;
    } else {
        ASN_Stream_EncodeBits(stream, data, 8);
    }
    return ASN_RESULT_OK;
}

#ifdef ASN_PRINT
static void ASN_String_Print(struct ASN_String_t* ThisPtr) {
    U32 i;

    ASN_PrintIndented("\"%s\" (0x", ThisPtr->data);

    for (i = 0; i < ThisPtr->length; i++) {
        ASN_Print("%02X", (U32)ThisPtr->data[i]);
    }
    ASN_Print(")" EOL);
}
#endif /* #ifdef ASN_PRINT */

// --------------------------------------------------------------------------
// Exported Function Definitions.

ASN_Result ASN_Length_Decode(ASN_Stream stream, U32* length, U32 low, U32 high) {
    // X.691 section 10.9
    BOOL isSet;
    U32 bitCount = ASN_U32CountBits(low, high);
#if !ASN_PER_ALIGN
    if (high != INT_MAX) {
        U32 base;

        // 10.9.4.2 unsupported
        if (high - low > 0xffff) {
            stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
            return &stream->error;
        }

        if (ASN_Stream_BitsLeft(stream) < bitCount) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        base = ASN_Stream_DecodeBits(stream, bitCount);

        // 10.9.4.1
        *length = low + base;

        // clamp value to upper limit
        if (*length > high) {
            *length = high;
        }

        return ASN_RESULT_OK;
    }
#endif

    // 10.9.3.3
    if (high < 65536) {
        return U32_Decode(length, 0, stream, low, high);
    }

    // 10.9.3.5
    ASN_Stream_CheckAndAlign(stream);

    if (BOOL_Decode(&isSet, 0, stream) != ASN_RESULT_OK) {
        return &stream->error;
    }

    if (isSet == 0) {
        // 10.9.3.8 unsupported
        if (ASN_Stream_BitsLeft(stream) < 7) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }

        // 10.9.3.6
        *length = ASN_Stream_DecodeBits(stream, 7);
    } else {
        if (BOOL_Decode(&isSet, 0, stream) != ASN_RESULT_OK) {
            return &stream->error;
        }
        if (isSet == 0) {
            // 10.9.3.8 unsupported
            if (ASN_Stream_BitsLeft(stream) < 14) {
                stream->error.error = ASN_ERROR_BUFFER_STARVING;
                return &stream->error;
            }

            // 10.9.3.7
            *length = ASN_Stream_DecodeBits(stream, 14);
        }
    }

    // clamp value to upper limit
    if (*length > high) {
        *length = high;
    }

    return ASN_RESULT_OK;
}

ASN_Result ASN_Length_Encode(ASN_Stream stream, U32 length, U32 low, U32 high) {
    // X.691 section 10.9
    U32 bitCount = ASN_U32CountBits(low, high);
#if !ASN_PER_ALIGN
    if (high != INT_MAX) {
        // 10.9.4.2 unsupported
        if (high - low > 0xffff) {
            stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
            return &stream->error;
        }

        // 10.9.4.1
        if (ASN_Stream_BitsLeft(stream) < bitCount) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        ASN_Stream_EncodeBits(stream, (ASN_BYTE)(length - low), bitCount);
        return ASN_RESULT_OK;
    }
#endif

    // 10.9.3.3
    if (high < 65536) {
        return U32_Encode(length, stream, low, high);
    }

    ASN_Stream_AlignOnByte(stream);

    if (length < 128) {
        // 10.9.3.6
        if (ASN_Stream_BitsLeft(stream) < 8) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        ASN_Stream_EncodeBits(stream, (ASN_BYTE)length, 8);
        return ASN_RESULT_OK;
    }

    if (BOOL_Encode(TRUE, stream) != ASN_RESULT_OK) {
        return &stream->error;
    }

    if (length < 0x4000) {
        // 10.9.3.7
        if (ASN_Stream_BitsLeft(stream) < 15) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        ASN_Stream_EncodeBits(stream, (ASN_BYTE)length, 15);
        return ASN_RESULT_OK;
    }

    if (BOOL_Encode(TRUE, stream) != ASN_RESULT_OK) {
        return &stream->error;
    }

    // 10.9.3.8 unsupported
    stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
    return &stream->error;
}

void* ASN_Malloc(ASN_Session session, U32 size) {
    void* pNew = session->malloc_next_ptr;

#ifdef ASN_ALIGNMENT
    if (size % ASN_ALIGNMENT) {
        // Align size
        size = ((size / ASN_ALIGNMENT) + 1) * ASN_ALIGNMENT;
    }
#endif /* #ifdef ASN_ALIGNMENT */

    if (session->malloc_next_ptr + size > session->malloc_end_ptr) return (void*)NULL;

    session->malloc_next_ptr += size;
    return pNew;
}

ASN_Result ASN_MultiByte_Decode(ASN_Stream stream, U32* value) {
    ASN_BYTE byte;
    *value = 0;
    do {
        if (ASN_Stream_BitsLeft(stream) < 8) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        /* shift and add in low order 7 bits */
        byte = ASN_Stream_DecodeByte(stream);
        *value = (*value << 7) + (byte & 0x7f);
    } while ((byte & 0x80) != 0);

    return ASN_RESULT_OK;
}

ASN_Result ASN_MultiByte_Encode(ASN_Stream stream, U32 value) {
    if (value < 128) {
        if (ASN_Stream_EncodeByte(stream, (ASN_BYTE)value) != ASN_RESULT_OK) {
            return &stream->error;
        }
    } else {
        U32 mask;
        U32 testmask;
        int bits;
        int testbits;

        mask = testmask = 0x7Fu; /* handle subid == 0 case */
        bits = testbits = 0u;

        while (testmask != 0) {
            if (value & testmask) { /* if any bits set */
                mask = testmask;
                bits = testbits;
            }
            testmask <<= 7;
            testbits += 7;
        }

        /* mask can't be zero here */
        while (mask != 0x7F) {
            /* fix a mask that got truncated above */
            if (mask == 0x1E00000) {
                mask = 0xFE00000;
            }

            if (ASN_Stream_EncodeByte(stream, (ASN_BYTE)((value & mask) >> bits) | 0x80) != ASN_RESULT_OK) {
                return &stream->error;
            }

            mask >>= 7;
            bits -= 7;
        }

        if (ASN_Stream_EncodeByte(stream, (ASN_BYTE)value & mask) != ASN_RESULT_OK) {
            return &stream->error;
        }
    }
    return ASN_RESULT_OK;
}

U32 ASN_MultiByte_EncodedSize(U32 value) {
    U32 encoded_size = 0;
    if (value < 128)
        encoded_size++;
    else {
        U32 mask;
        U32 testmask;

        mask = testmask = 0x7Fu; /* handle subid == 0 case */

        while (testmask != 0) {
            if (value & testmask) /* if any bits set */
            {
                mask = testmask;
            }
            testmask <<= 7;
        }

        /* mask can't be zero here */
        while (mask != 0x7F) {
            /* fix a mask that got truncated above */
            if (mask == 0x1E00000) {
                mask = 0xFE00000;
            }
            encoded_size++;
            mask >>= 7;
        }

        encoded_size++;
    }

    return encoded_size;
}

void ASN_ObjectId_Associate(ASN_ObjectId ThisPtr, U16* data, U16 arcs) {
    ThisPtr->data = data;
    ThisPtr->length = arcs;
}

ASN_ObjectId ASN_ObjectId_Create(ASN_Session session) {
    ASN_ObjectId ThisPtr = (ASN_ObjectId)ASN_Malloc(session, sizeof(struct ASN_ObjectId_t));
    if (ThisPtr != (ASN_ObjectId)NULL) {
        ThisPtr->data = NULL;
        ThisPtr->length = 0;
        ThisPtr->isRelative = FALSE;
    }
    return ThisPtr;
}

ASN_Result ASN_ObjectId_Decode(ASN_ObjectId ThisPtr, ASN_Session session, ASN_Stream stream) {
    U32 i;
    U32 bitCount;
    U32 subId;
    U16 length;
    U32 encoded_bytes;
    bitCount = ASN_U32CountBits(0, 7);

    if (ASN_Stream_BitsLeft(stream) < bitCount) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }

    if (ASN_MultiByte_Decode(stream, &encoded_bytes) != ASN_RESULT_OK) {
        return &stream->error;
    }

    if (ASN_Stream_BitsLeft(stream) < encoded_bytes * 8) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }

    length = 0u;
    for (i = stream->bytePointer; (i - stream->bytePointer) < encoded_bytes; i++) {
        U32 currentByte = stream->startPtr[i];
        currentByte <<= (8 - stream->bitPointer);
        if (~currentByte & 0x80u) {
            length++;
        }
    }

    if ((ThisPtr->data != NULL)) {
        U16 associated_length = ThisPtr->length;

        // Internally, length will represent the number of times we need to decode a multibyte.
        // Externally this is the same if relative oid else it will be 1+length.
        ThisPtr->length = length;

        // In absolute object identifiers the first value represents two output values
        // Thus we need to add 1 to the length in this case.
        ThisPtr->length += ThisPtr->isRelative == FALSE ? 1 : 0;

        // Check if a bad association has been made.
        if (associated_length != ThisPtr->length) {
            session->error.error = ASN_ERROR_MEMORY;
            return &session->error;
        }
    } else {
        // Internally, length will represent the number of times we need to decode a multibyte.
        // Externally this is the same if relative oid else it will be 1+length.
        ThisPtr->length = length;

        // In absolute object identifiers the first value represents two output values
        // Thus we need to add 1 to the length in this case.
        ThisPtr->length += ThisPtr->isRelative == FALSE ? 1 : 0;

        ThisPtr->data = (U16*)ASN_Malloc(session, 2 * ThisPtr->length);
        if (ThisPtr->data == NULL) {
            session->error.error = ASN_ERROR_MEMORY;
            return &session->error;
        }
    }

    memset(ThisPtr->data, 0x0u, 2 * ThisPtr->length);

    // handle zero length strings correctly
    if (ThisPtr->length == 0) return ASN_RESULT_OK;

    // start at the second identifier in the buffer, because we will later
    // expand the first number into the first two IDs
    i = ThisPtr->isRelative == FALSE ? 1 : 0;
    while (length > 0) {
        U32 tmp;  // tmp would be unneeded if ThisPtr->data was 32 bits or higher.
        if (ASN_MultiByte_Decode(stream, &tmp) != ASN_RESULT_OK) {
            return &stream->error;
        }
        ThisPtr->data[i++] = (U16)tmp;
        length--;
    }

    if (ThisPtr->isRelative == TRUE) {
        return ASN_RESULT_OK;
    }

    /*
    * The first two subidentifiers are encoded into the first component
    * with the value (X * 40) + Y, where:
    *  X is the value of the first subidentifier.
    *  Y is the value of the second subidentifier.
    */
    subId = ThisPtr->data[1];
    if (subId < 40) {
        ThisPtr->data[0] = 0;
        ThisPtr->data[1] = (U16)subId;
    } else if (subId < 80) {
        ThisPtr->data[0] = 1;
        ThisPtr->data[1] = (U16)subId - 40;
    } else {
        ThisPtr->data[0] = 2;
        ThisPtr->data[1] = (U16)subId - 80;
    }

    return ASN_RESULT_OK;
}

ASN_Result ASN_ObjectId_Encode(ASN_ObjectId ThisPtr, ASN_Stream stream) {
    U32 length;
    U32 subId;
    U32 oidSize;
    U16* objId;

    objId = ThisPtr->data;
    length = ThisPtr->length;

    if (ThisPtr->isRelative == FALSE) {
        if ((objId == NULL) || (length < 2)) {
            stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
            return &stream->error;
        }
        subId = objId[0] * 40 + objId[1];
        objId += 2;
    } else {
        // In relative OIDs the two first values are encoded seperatly
        // thus need of running one extra loop.
        subId = objId[0];
        length++;
        objId++;
    }

    // Calculate the number of bytes that will be encoded.
    if (ASN_Stream_BitsLeft(stream) < (oidSize = ASN_ObjectId_EncodedSize(ThisPtr))) {
        stream->error.error = ASN_ERROR_BUFFER_FULL;
        return &stream->error;
    }

    // Encode the oidSize
    oidSize = oidSize / 8 - ASN_MultiByte_EncodedSize(length);
    ASN_MultiByte_Encode(stream, oidSize);

    // Encode the oid
    while (--length > 0) {
        ASN_MultiByte_Encode(stream, subId);
        if (length > 1) subId = *objId++;
    }

    return ASN_RESULT_OK;
}

U32 ASN_ObjectId_EncodedSize(ASN_ObjectId ThisPtr) {
    U32 length;
    U32 encoded_size;
    U32 subId;
    U16* objId = ThisPtr->data;
    length = ThisPtr->length;
    encoded_size = 0u;

    if (ThisPtr->isRelative == FALSE) {
        if ((objId == NULL) || (length < 2)) {
            return encoded_size;
        }
        subId = objId[0] * 40 + objId[1];
        objId += 2;
        encoded_size = 1u;
    } else {
        // In relative OIDs the two first values are encoded seperatly
        // thus need of running one extra loop.
        subId = objId[0];
        length++;
        objId++;
    }

    // Calculate number of bytes used to encode the length of the oid.
    encoded_size += ASN_MultiByte_EncodedSize(length);

    // Calculate number of bytes used to encode the oio
    while (--length > 0) {
        encoded_size += ASN_MultiByte_EncodedSize(subId);

        if (length > 1) subId = *objId++;
    }

    // Return the number of bits needed to encode the oid
    return encoded_size * 8;
}

U32 ASN_ObjectId_SessionSize() { return sizeof(struct ASN_ObjectId_t); }

void ASN_OctetString_Associate(ASN_OctetString ThisPtr, ASN_BYTE* data, U32 length) {
    ThisPtr->data = data;
    ThisPtr->length = length;
}

void ASN_OctetString_AssociateText(ASN_OctetString ThisPtr, char* text) {
    ThisPtr->data = (ASN_BYTE*)text;
    ThisPtr->length = strlen(text);
}

ASN_OctetString ASN_OctetString_Create(ASN_Session session) {
    ASN_OctetString ThisPtr = (ASN_OctetString)ASN_Malloc(session, sizeof(struct ASN_String_t));
    if (ThisPtr != (ASN_OctetString)NULL) {
        ThisPtr->data = NULL;
        ThisPtr->length = 0;
    }
    return ThisPtr;
}

ASN_Result ASN_OctetString_Decode(ASN_OctetString ThisPtr, ASN_Session session, ASN_Stream stream, U32 low, U32 high) {
    U32 i;
    U32 bitCount = ASN_U32CountBits(low, high);

    if (ASN_Stream_BitsLeft(stream) < bitCount) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }

    // Get Number of bytes
    ThisPtr->length = ASN_Stream_DecodeInt(stream, bitCount);
    ThisPtr->length += low;

    if ((ThisPtr->length > high) || (ThisPtr->length < low)) {
        stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return &stream->error;
    }

    if ((U32)ASN_Stream_BitsLeft(stream) < ThisPtr->length * 8) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }

    ThisPtr->data = (unsigned char*)ASN_Malloc(session, ThisPtr->length + 1);
    if (ThisPtr->data == NULL) {
        stream->error.error = ASN_ERROR_MEMORY;
        return &stream->error;
    }

    for (i = 0; i < ThisPtr->length; i++) {
        ThisPtr->data[i] = ASN_Stream_DecodeByte(stream);
    }
    ThisPtr->data[ThisPtr->length] = (ASN_BYTE)'\0';
    return ASN_RESULT_OK;
}

ASN_Result ASN_OctetString_Encode(ASN_OctetString ThisPtr, ASN_Stream stream, U32 low, U32 high) {
    ASN_Result result;
    U32 i;
    U32 bitCount = ASN_U32CountBits(low, high);

    if ((ThisPtr->length > high) || (ThisPtr->length < low)) {
        stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
        return &stream->error;
    }

    if ((U32)ASN_Stream_BitsLeft(stream) < bitCount + ThisPtr->length * 8) {
        stream->error.error = ASN_ERROR_BUFFER_FULL;
        return &stream->error;
    }

    result = ASN_Stream_EncodeInt(stream, ThisPtr->length - low, bitCount);
    if (result != NULL) return result;

    for (i = 0; i < ThisPtr->length; i++) {
        result = ASN_Stream_EncodeByte(stream, ThisPtr->data[i]);
        if (result != NULL) return result;
    }
    return ASN_RESULT_OK;
}

U32 ASN_OctetString_EncodedSize(ASN_OctetString ThisPtr, U32 low, U32 high) {
    return ASN_U32CountBits(low, high) + (ThisPtr->length * 8);
}

U32 ASN_OctetString_Get(ASN_OctetString ThisPtr, ASN_BYTE** data) {
    *data = ThisPtr->data;
    return ThisPtr->length;
}

char* ASN_OctetString_GetText(ASN_OctetString ThisPtr) {
    if (ThisPtr->data[ThisPtr->length] == (ASN_BYTE)'\0')
        return (char*)ThisPtr->data;
    else
        return (char*)NULL;
}

#ifdef ENABLE_ASN_SESSION_SIZE

U32 ASN_OctetString_SessionSize(U32 low, U32 high) {
    low;
    /* Adding 1 because we allocate one byte extra for a null termination character. */
    return sizeof(struct ASN_String_t) + high + 1;
}
#endif /* #ifdef ENABLE_ASN_SESSION_SIZE */

ASN_Result ASN_OctetString_Set(ASN_OctetString ThisPtr, ASN_Session session, ASN_BYTE* data, U32 length) {
    ASN_Result result = ASN_RESULT_OK;
    U32 i;

    ThisPtr->data = (unsigned char*)ASN_Malloc(session, length + 1);
    if (ThisPtr->data == (ASN_BYTE*)NULL) {
        result = ASN_Session_GetResult(session);
        result->error = ASN_ERROR_MEMORY;
        return result;
    }
    ThisPtr->length = length;
    for (i = 0; i < length; i++) {
        ThisPtr->data[i] = data[i];
    }
    ThisPtr->data[length] = (ASN_BYTE)'\0';
    return ASN_RESULT_OK;
}

ASN_Result ASN_OctetString_SetText(ASN_OctetString ThisPtr, ASN_Session session, char* text) {
    ASN_Result result = ASN_RESULT_OK;
    U32 length = strlen(text);

    ThisPtr->data = (unsigned char*)ASN_Malloc(session, length + 1);
    if (ThisPtr->data == (ASN_BYTE*)NULL) {
        result = ASN_Session_GetResult(session);
        result->error = ASN_ERROR_MEMORY;
        return result;
    }

    strcpy((char*)ThisPtr->data, text);
    ThisPtr->length = length;
    return ASN_RESULT_OK;
}

void ASN_PrintableString_Associate(ASN_PrintableString ThisPtr, ASN_BYTE* data, U32 length) {
    ThisPtr->data = data;
    ThisPtr->length = length;
}

void ASN_PrintableString_AssociateText(ASN_PrintableString ThisPtr, char* text) {
    ThisPtr->data = (ASN_BYTE*)text;
    ThisPtr->length = strlen(text);
}

U32 ASN_PrintableString_BitsPerCharacter(U32 low, U32 high) {
    U32 bitsPerCharacter = ASN_U32CountBits(low, high);
#if ASN_PER_ALIGN        // If using PER aligned, make sure its a power of 2
    bitsPerCharacter--;  // Make sure power of 2:s themself work
    bitsPerCharacter = (bitsPerCharacter >> 1) | bitsPerCharacter;
    bitsPerCharacter = (bitsPerCharacter >> 2) | bitsPerCharacter;
    bitsPerCharacter = (bitsPerCharacter >> 4) | bitsPerCharacter;
    bitsPerCharacter = (bitsPerCharacter >> 8) | bitsPerCharacter;
    bitsPerCharacter = (bitsPerCharacter >> 16) | bitsPerCharacter;
    bitsPerCharacter++;  // Flip to make a power of 2. Ex, 0111 is flipped to 1000
#endif                   /* #ifdef ASN_PER_ALIGN */
    return bitsPerCharacter;
}

ASN_PrintableString ASN_PrintableString_Create(ASN_Session session) {
    ASN_PrintableString ThisPtr = (ASN_PrintableString)ASN_Malloc(session, sizeof(struct ASN_PrintableString_t));
    if (ThisPtr != (ASN_PrintableString)NULL) {
        ThisPtr->data = NULL;
        ThisPtr->length = 0;
        ThisPtr->characterSet = (unsigned char*)DEFAULT_CHARACTER_SET;
        ThisPtr->characterSetSize = strlen(DEFAULT_CHARACTER_SET);
    }
    return ThisPtr;
}

ASN_Result ASN_PrintableString_Decode(ASN_PrintableString ThisPtr, ASN_Session session, ASN_Stream stream, U32 low,
                                      U32 high, const char* characterSet) {
    U32 i;
    ASN_Result result;
    ASN_BYTE charSetMinMax[2];
    U32 bitCount;
    U32 characterSetLength;

    if (characterSet == NULL) {
        characterSet = DEFAULT_CHARACTER_SET;
    }
    characterSetLength = strlen(characterSet);
    bitCount = ASN_PrintableString_BitsPerCharacter(1, characterSetLength);

    result = ASN_PrintableString_SetCharacterSet(ThisPtr, (ASN_BYTE*)characterSet, characterSetLength);

    if (result != ASN_RESULT_OK) {
        return result;
    }

    // Number of bits left in stream < BitsInEncodedInt + Characters*BitsPerCharacter
    if (ASN_Stream_BitsLeft(stream) < ASN_U32CountBits(low, high) + ThisPtr->length * bitCount) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }

    ThisPtr->length = ASN_Stream_DecodeInt(stream, ASN_PrintableString_BitsPerCharacter(low, high));
    ThisPtr->length += low;

    if ((ThisPtr->length > high) || (ThisPtr->length < low)) {
        stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return &stream->error;
    }

    ThisPtr->data = (unsigned char*)ASN_Malloc(session, ThisPtr->length + 1);
    if (ThisPtr->data == NULL) {
        stream->error.error = ASN_ERROR_MEMORY;
        return &stream->error;
    }

    // Check if data has been indexed
    ASN_PrintableString_MinMax(&charSetMinMax[0], &charSetMinMax[1], (ASN_BYTE*)characterSet);

    for (i = 0; i < ThisPtr->length; i++) {
        ThisPtr->data[i] = (ASN_BYTE)ASN_Stream_DecodeBits(stream, bitCount);

        if (ASN_DO_INDEXING(charSetMinMax[1], bitCount) == TRUE) {
            ThisPtr->data[i] = characterSet[(U32)ThisPtr->data[i]];
        }
    }

    ThisPtr->data[ThisPtr->length] = (ASN_BYTE)'\0';

    return ASN_RESULT_OK;
}

ASN_Result ASN_PrintableString_Encode(ASN_PrintableString ThisPtr, ASN_Stream stream, U32 low, U32 high,
                                      const char* characterSet) {
    ASN_Result result;
    U32 i;
    ASN_BYTE charSetMinMax[2];
    ASN_BYTE charToEncode;
    U32 bitCount;
    U32 characterSetLength;

    if (characterSet == NULL) {
        characterSet = DEFAULT_CHARACTER_SET;
    }
    characterSetLength = strlen(characterSet);
    bitCount = ASN_PrintableString_BitsPerCharacter(1, characterSetLength);

    result = ASN_PrintableString_SetCharacterSet(ThisPtr, (ASN_BYTE*)characterSet, characterSetLength);

    if (result != ASN_RESULT_OK) {
        return result;
    }

    if ((low > ThisPtr->length) || (ThisPtr->length > high)) {
        stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
        return &stream->error;
    }

    if (ASN_PrintableString_IsInCharacterSet(ThisPtr, (ASN_BYTE*)ThisPtr->data, ThisPtr->length) == FALSE) {
        result = &stream->error;
        result->error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return result;
    }

    // Number of bits left in stream < BitsInEncodedInt + Characters*BitsPerCharacter
    if ((U32)ASN_Stream_BitsLeft(stream) < ASN_U32CountBits(low, high) + ThisPtr->length * bitCount) {
        stream->error.error = ASN_ERROR_BUFFER_FULL;
        return &stream->error;
    }

    result = ASN_Stream_EncodeInt(stream, ThisPtr->length - low, ASN_PrintableString_BitsPerCharacter(low, high));
    if (result != NULL) return result;

    // Check if indexing will be needed
    ASN_PrintableString_MinMax(&charSetMinMax[0], &charSetMinMax[1], (ASN_BYTE*)characterSet);

    for (i = 0; i < ThisPtr->length; i++) {
        if (ASN_DO_INDEXING(charSetMinMax[1], bitCount) == TRUE)
            charToEncode = ASN_PrintableString_FindIndexing(ThisPtr, i);
        else
            charToEncode = ThisPtr->data[i];

        result = ASN_Stream_EncodeBits(stream, charToEncode, bitCount);
        if (result != NULL) {
            return result;
        }
    }

    return ASN_RESULT_OK;
}

U32 ASN_PrintableString_EncodedSize(ASN_PrintableString ThisPtr, U32 low, U32 high, const char* characterSet) {
    U32 length = characterSet == NULL ? strlen(DEFAULT_CHARACTER_SET) : strlen(characterSet);
    return ASN_PrintableString_BitsPerCharacter(low, high) +
           (ThisPtr->length * ASN_PrintableString_BitsPerCharacter(1, length));
}

ASN_BYTE ASN_PrintableString_FindIndexing(ASN_PrintableString ThisPtr, U32 index) {
    U32 i;

    for (i = 0; i < ThisPtr->characterSetSize; i++) {
        if (ThisPtr->data[index] == ThisPtr->characterSet[i]) return (ASN_BYTE)i;
    }

    return (ASN_BYTE)ThisPtr->characterSetSize;
}

U32 ASN_PrintableString_Get(ASN_PrintableString ThisPtr, ASN_BYTE** data) {
    *data = ThisPtr->data;
    return ThisPtr->length;
}

char* ASN_PrintableString_GetText(ASN_PrintableString ThisPtr) {
    if ((char*)ThisPtr->data == (char*)NULL) return (char*)NULL;
    if (ThisPtr->data[ThisPtr->length] == (ASN_BYTE)'\0')
        return (char*)ThisPtr->data;
    else
        return (char*)NULL;
}

BOOL ASN_PrintableString_IsInCharacterSet(ASN_PrintableString ThisPtr, ASN_BYTE* chars, U32 numberOfCharacters) {
    U32 i;
    U32 j;
    BOOL returnValue = FALSE;

    for (i = 0; i < numberOfCharacters; i++) {
        returnValue = FALSE;
        for (j = 0; j < ThisPtr->characterSetSize; j++) {
            if (chars[i] == ThisPtr->characterSet[j]) {
                returnValue = TRUE;
                break;
            }
        }
        if (returnValue == FALSE) {
            break;
        }
    }

    return returnValue;
}

void ASN_PrintableString_MinMax(ASN_BYTE* charSetMinValue, ASN_BYTE* charSetMaxValue, ASN_BYTE* charSet) {
    U32 i;
    U32 characterSetLength = charSet == NULL ? 0u : strlen((const char*)charSet);
    *charSetMinValue = 255u;
    *charSetMaxValue = 0u;
    for (i = 0; i < characterSetLength; i++) {
        if (*charSetMinValue > charSet[i]) {
            *charSetMinValue = charSet[i];
        }

        if (*charSetMaxValue < charSet[i]) {
            *charSetMaxValue = charSet[i];
        }
    }
}

#ifdef ENABLE_ASN_SESSION_SIZE
U32 ASN_PrintableString_SessionSize(U32 low, U32 high, const char* characterSet) {
    // Using the variable low for storing intermediate calculations (since it already exists).
    low = characterSet == NULL ? strlen(DEFAULT_CHARACTER_SET) : strlen(characterSet);
    /* Adding 1 because we allocate one byte extra for a null termination character. */
    return sizeof(struct ASN_PrintableString_t) + high + 1 + low;
}
#endif /* #ifdef ENABLE_ASN_SESSION_SIZE */

ASN_Result ASN_PrintableString_Set(ASN_PrintableString ThisPtr, ASN_Session session, ASN_BYTE* data, U32 length) {
    ASN_Result result = ASN_RESULT_OK;
    U32 i;

    if (ThisPtr->characterSet == NULL) {
        ThisPtr->characterSet = (unsigned char*)DEFAULT_CHARACTER_SET;
        ThisPtr->characterSetSize = strlen(DEFAULT_CHARACTER_SET);
    }

    if (ASN_PrintableString_IsInCharacterSet(ThisPtr, data, length) == FALSE) {
        result = ASN_Session_GetResult(session);
        result->error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return result;
    }

    ThisPtr->data = (unsigned char*)ASN_Malloc(session, length + 1);
    if (ThisPtr->data == (ASN_BYTE*)NULL) {
        result = ASN_Session_GetResult(session);
        result->error = ASN_ERROR_MEMORY;
        return result;
    }
    ThisPtr->length = length;
    for (i = 0; i < length; i++) {
        ThisPtr->data[i] = data[i];
    }
    ThisPtr->data[length] = (ASN_BYTE)'\0';
    return ASN_RESULT_OK;
}

ASN_Result ASN_PrintableString_SetCharacterSet(ASN_PrintableString ThisPtr, ASN_BYTE* charSet, U32 charSetSize) {
    ASN_Result result = ASN_RESULT_OK;
    ThisPtr->characterSet = (ASN_BYTE*)DEFAULT_CHARACTER_SET;
    ThisPtr->characterSetSize = strlen((const char*)ThisPtr->characterSet);

    // Check if the user whiches to use the default charset.
    if (charSet == NULL) {
        return result;
    }

    // Check so that we are not attempting to set a charSet outside the limits of a PrintableString
    if (ASN_PrintableString_IsInCharacterSet(ThisPtr, charSet, charSetSize) == true) {
        ThisPtr->characterSet = (ASN_BYTE*)charSet;
        ThisPtr->characterSetSize = charSetSize;
    } else {
        result->error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
    }
    return result;
}

ASN_Result ASN_PrintableString_SetText(ASN_PrintableString ThisPtr, ASN_Session session, char* text) {
    ASN_Result result = ASN_RESULT_OK;
    U32 length = strlen(text);

    if (ThisPtr->characterSet == NULL) {
        ThisPtr->characterSet = (unsigned char*)DEFAULT_CHARACTER_SET;
        ThisPtr->characterSetSize = strlen(DEFAULT_CHARACTER_SET);
    }

    if (ASN_PrintableString_IsInCharacterSet(ThisPtr, (ASN_BYTE*)text, length) == FALSE) {
        result = ASN_Session_GetResult(session);
        result->error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return result;
    }

    ThisPtr->data = (ASN_BYTE*)ASN_Malloc(session, length + 1);
    if (ThisPtr->data == (ASN_BYTE*)NULL) {
        result = ASN_Session_GetResult(session);
        result->error = ASN_ERROR_MEMORY;
        return result;
    }

    strcpy((char*)ThisPtr->data, text);
    ThisPtr->length = length;
    return result;
}

void ASN_RelativeObjectId_Associate(ASN_RelativeObjectId ThisPtr, U16* data, U16 arcs) {
    ASN_ObjectId_Associate(ThisPtr, data, arcs);
}

ASN_RelativeObjectId ASN_RelativeObjectId_Create(ASN_Session session) {
    ASN_RelativeObjectId relOID = ASN_ObjectId_Create(session);
    if (relOID != (ASN_RelativeObjectId)NULL) {
        relOID->isRelative = TRUE;
    }
    return relOID;
}

ASN_Result ASN_RelativeObjectId_Decode(ASN_RelativeObjectId ThisPtr, ASN_Session session, ASN_Stream stream) {
    return ASN_ObjectId_Decode(ThisPtr, session, stream);
}

ASN_Result ASN_RelativeObjectId_Encode(ASN_RelativeObjectId ThisPtr, ASN_Stream stream) {
    return ASN_ObjectId_Encode(ThisPtr, stream);
}

U32 ASN_RelativeObjectId_EncodedSize(ASN_RelativeObjectId ThisPtr) {
    // Absolute objectids encode the 2 first values together with each other.
    return ASN_ObjectId_EncodedSize(ThisPtr);
}

U32 ASN_RelativeObjectId_SessionSize() { return ASN_ObjectId_SessionSize(); }

U32 ASN_S32CountBits(S32 low, S32 high) {
    U32 range = 0;
    U32 nBits = 0;

    if (high < 0) {
        range -= low;
        range += high;
    } else {
        range += high;
        range -= low;
    }
    while (range) {
        range = range >> 1;
        nBits++;
    }
    return nBits;
}

U32 ASN_Session_BytesLeft(ASN_Session session) { return session->malloc_end_ptr - session->malloc_next_ptr; }

void ASN_Session_ClearError(ASN_Session session) {
    session->error.error = ASN_ERROR_NO_ERROR;
    session->error.linenumber = 0;
}

ASN_Session ASN_Session_Create(ASN_BYTE* decoded_buffer, U32 buf_size) {
    ASN_Session session = (ASN_Session)decoded_buffer;
    if (buf_size < ASN_SESSION_SIZE) {
        return (ASN_Session)NULL;
    }
    session->malloc_buf_size = buf_size;
    session->malloc_buffer = decoded_buffer + sizeof(struct ASN_Session_t);
    session->malloc_next_ptr = session->malloc_buffer;
    session->malloc_end_ptr = decoded_buffer + session->malloc_buf_size;
    session->error.error = ASN_ERROR_NO_ERROR;
    session->error.linenumber = 0;
    return session;
}

ASN_Result ASN_Session_GetResult(ASN_Session session) {
    session->error.error = ASN_ERROR_NO_ERROR;
    session->error.linenumber = 0;
    return &session->error;
}

void ASN_Session_Reset(ASN_Session session) {
    session->malloc_next_ptr = session->malloc_buffer;
    session->error.error = ASN_ERROR_NO_ERROR;
    session->error.linenumber = 0;
}

ASN_Result ASN_SmallUnsigned_Decode(ASN_Stream stream, U32* value) {
    // X.691 Section 10.6
    BOOL valueIsGreaterThan63;
    U32 len = 0u;

    if (BOOL_Decode(&valueIsGreaterThan63, 0, stream) != ASN_RESULT_OK) {
        return &stream->error;
    }

    // 10.6.1
    if (valueIsGreaterThan63 == FALSE) {
        if (ASN_Stream_BitsLeft(stream) < 6) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        *value = ASN_Stream_DecodeBits(stream, 6);
        return ASN_RESULT_OK;
    }

    // 10.6.2
    if (ASN_Length_Decode(stream, &len, 0, UINT32_MAX) != ASN_RESULT_OK) {
        return &stream->error;
    }

    if (ASN_Stream_BitsLeft(stream) < len * 8) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }
    *value = ASN_Stream_DecodeBits(stream, len * 8);

    return ASN_RESULT_OK;
}

ASN_Result ASN_SmallUnsigned_Encode(ASN_Stream stream, U32 value) {
    // X.691 Section 10.6
    U32 len = 4;

    // 10.6.1
    if (value <= 63) {
        if (ASN_Stream_BitsLeft(stream) < 7) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        ASN_Stream_EncodeBits(stream, (ASN_BYTE)value, 7);
        return ASN_RESULT_OK;
    }

    // 10.6.2
    if (BOOL_Encode(TRUE, stream) != ASN_RESULT_OK) {
        return &stream->error;
    }

    if (value < 256) {
        len = 1;
    } else if (value < 65536) {
        len = 2;
    } else if (value < 0x1000000) {
        len = 3;
    }

    // 10.9
    ASN_Length_Encode(stream, len, 0, UINT32_MAX);

    if (ASN_Stream_BitsLeft(stream) < len * 8) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }
    ASN_Stream_EncodeBits(stream, (ASN_BYTE)value, len * 8);
    return ASN_RESULT_OK;
}

U32 ASN_SmallUnsigned_EncodedSize(U32 value) {
    U32 bitCount;
    U32 len = 4;

    // 10.6.1
    if (value <= 63) {
        return 7u;
    }

    // 10.6.2
    if (value < 256) {
        len = 1;
    } else if (value < 65536) {
        len = 2;
    } else if (value < 0x1000000) {
        len = 3;
    }

    // 10.9
    bitCount = 1u;
    bitCount += len * 8;
    bitCount += ASN_U32CountBits(0, UINT32_MAX);

    return bitCount;
}

void ASN_Stream_AlignOnByte(ASN_Stream stream) {
    if (stream->bitPointer != 8) ASN_Stream_EncodeBits(stream, (ASN_BYTE)0, stream->bitPointer);
}

void ASN_Stream_AttachBuffer(ASN_Stream stream, ASN_BYTE* buffer, U32 size) {
    ASN_Stream_Reset(stream);
    stream->startPtr = buffer;
    stream->endPtr = buffer + size;
}

U32 ASN_Stream_BitsLeft(ASN_Stream stream) {
    // ASN_BYTE;
    // ASN_Stream_t;
    // return 0;
    U32 retVal = ((U32)(stream->endPtr - stream->startPtr) - stream->bytePointer) * 8u + stream->bitPointer - 8;
    return retVal;
    // return ( (U32)(stream->endPtr - stream->startPtr) - stream->bytePointer ) * 8 + stream->bitPointer - 8;
}

U32 ASN_Stream_BitsUsed(ASN_Stream stream) { return stream->bytePointer * 8 + (8 - stream->bitPointer); }

U32 ASN_Stream_BytesUsed(ASN_Stream stream) {
    return (stream->bitPointer == 8) ? stream->bytePointer : stream->bytePointer + 1;
}

void ASN_Stream_ClearError(ASN_Stream stream) {
    stream->error.error = ASN_ERROR_NO_ERROR;
    stream->error.linenumber = 0;
}

ASN_Stream ASN_Stream_Create(ASN_BYTE* buffer, U32 size) {
    ASN_Stream stream = (ASN_Stream)buffer;
    if (size < ASN_STREAM_SIZE) {
        return (ASN_Stream)NULL;
    }
    if (size > sizeof(struct ASN_Stream_t)) {
        stream->startPtr = buffer + sizeof(struct ASN_Stream_t);
        stream->endPtr = buffer + size;
    } else {
        stream->startPtr = NULL;
        stream->endPtr = NULL;
    }
    stream->bytePointer = 0;
    stream->bitPointer = 8;
    stream->error.error = ASN_ERROR_NO_ERROR;
    stream->error.linenumber = 0;
    return stream;
}

ASN_Result ASN_Stream_EncodeInt(ASN_Stream stream, U32 value, U32 bitCount) {
    ASN_BYTE valuePart;
    ASN_Result result;
    U32 mask;

    if (bitCount > 32) {
        stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
        return &stream->error;
    }

    if (bitCount == 0) {
        return ASN_RESULT_OK;
    }

    // Select the bitCount least significant bits
    mask = ~(~0L << bitCount);

    if ((bitCount != 32) && (value != (value & mask))) {
        stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return &stream->error;
    }
    while (bitCount > 8) {
        mask = 0xff << (bitCount - 8);
        valuePart = (ASN_BYTE)((value & mask) >> (bitCount - 8));
        result = ASN_Stream_EncodeByte(stream, valuePart);
        if (result != (ASN_Result)ASN_RESULT_OK) {
            return result;
        }
        bitCount -= 8;
    }
    mask = (~(~0L << bitCount)) & 0xff;
    valuePart = (ASN_BYTE)(value & mask);
    return ASN_Stream_EncodeBits(stream, valuePart, bitCount);
}

ASN_BYTE* ASN_Stream_GetBuffer(ASN_Stream stream, U32* size) {
    *size = ASN_Stream_BytesUsed(stream);
    return stream->startPtr;
}

ASN_Result ASN_Stream_GetResult(ASN_Stream stream) { return &stream->error; }

void ASN_Stream_Reset(ASN_Stream stream) {
    stream->error.error = ASN_ERROR_NO_ERROR;
    stream->error.linenumber = 0;
    stream->bytePointer = 0;
    stream->bitPointer = 8;
}

U32 ASN_U32CountBits(U32 low, U32 high) {
    U32 range = high - low;
    U32 nBits = 0;
    while (range) {
        range = range >> 1;
        nBits++;
    }
    return nBits;
}

void ASN_UTF8String_Associate(ASN_UTF8String ThisPtr, ASN_BYTE* data, U32 length) {
    ASN_OctetString_Associate((ASN_OctetString)ThisPtr, data, length);
}

void ASN_UTF8String_AssociateText(ASN_UTF8String ThisPtr, char* text) {
    ASN_OctetString_AssociateText((ASN_OctetString)ThisPtr, text);
}

ASN_UTF8String ASN_UTF8String_Create(ASN_Session session) { return (ASN_UTF8String)ASN_OctetString_Create(session); }

ASN_Result ASN_UTF8String_Decode(ASN_UTF8String ThisPtr, ASN_Session session, ASN_Stream stream, U32 low, U32 high) {
    U32 i;
    U32 bitCount = ASN_U32CountBits(low, high);
    U32 firstBit;

    if (ASN_Stream_BitsLeft(stream) < bitCount) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }
    // Decode length
    firstBit = ASN_Stream_DecodeBits(stream, 1);
    if (firstBit == 0) {
        if ((U32)ASN_Stream_BitsLeft(stream) < 7) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }

        // First form
        ThisPtr->length = ASN_Stream_DecodeInt(stream, 7);
        ThisPtr->length += low;

        if ((ThisPtr->length > high) || (ThisPtr->length < low)) {
            stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
            return &stream->error;
        }
    } else {
        U32 secondBit;
        if ((U32)ASN_Stream_BitsLeft(stream) < 1) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        secondBit = ASN_Stream_DecodeBits(stream, 1);
        if (secondBit == 0) {
            // Second form
            if ((U32)ASN_Stream_BitsLeft(stream) < 14) {
                stream->error.error = ASN_ERROR_BUFFER_STARVING;
                return &stream->error;
            }
            ThisPtr->length = ASN_Stream_DecodeInt(stream, 14);
            ThisPtr->length += low;

            if ((ThisPtr->length > high) || (ThisPtr->length < low)) {
                stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
                return &stream->error;
            }
        } else {
            // Third form
            // if ( ( U32)ASN_Stream_BitsLeft( stream ) < 6 )
            //{
            //   stream->error.error = ASN_ERROR_BUFFER_STARVING;
            //   return &stream->error;
            //}
            // ThisPtr->length = ASN_Stream_DecodeInt(stream,6) * 16384;
            stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
            return &stream->error;
        }
    }

    if ((U32)ASN_Stream_BitsLeft(stream) < ThisPtr->length * 8) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }

    ThisPtr->data = (unsigned char*)ASN_Malloc(session, ThisPtr->length + 1);
    if (ThisPtr->data == NULL) {
        stream->error.error = ASN_ERROR_MEMORY;
        return &stream->error;
    }

    for (i = 0; i < ThisPtr->length; i++) {
        ThisPtr->data[i] = ASN_Stream_DecodeByte(stream);
    }
    ThisPtr->data[ThisPtr->length] = (ASN_BYTE)'\0';
    return ASN_RESULT_OK;
}

ASN_Result ASN_UTF8String_Encode(ASN_UTF8String ThisPtr, ASN_Stream stream, U32 low, U32 high) {
    U32 i;
    ASN_Result result;
    U32 bitCount = ASN_U32CountBits(low, high);

    if ((ThisPtr->length > high) || (ThisPtr->length < low)) {
        stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
        return &stream->error;
    }

    // ThisPtr->length is actually number of bytes.. Confusing I know.
    if (ThisPtr->length < 128) {
        // First form
        U32 secondBit;
        if ((U32)ASN_Stream_BitsLeft(stream) < 1) {
            stream->error.error = ASN_ERROR_BUFFER_STARVING;
            return &stream->error;
        }
        result = ASN_Stream_EncodeBits(stream, 0, 1);
        if (result != ASN_RESULT_OK) {
            return result;
        }
        result = ASN_Stream_EncodeInt(stream, ThisPtr->length - low, 7);
        if (result != ASN_RESULT_OK) {
            return result;
        }
    } else if (ThisPtr->length < 16384) {
        // Second form
        result = ASN_Stream_EncodeBits(stream, 1, 1);
        if (result != ASN_RESULT_OK) {
            return result;
        }

        result = ASN_Stream_EncodeBits(stream, 0, 1);
        if (result != ASN_RESULT_OK) {
            return result;
        }

        result = ASN_Stream_EncodeInt(stream, ThisPtr->length - low, 14);
        if (result != ASN_RESULT_OK) {
            return result;
        }
    } else {
        // Third form
        // encodeBits((byte)1, 1);
        // encodeBits((byte)1, 1);
        // int multiplier = getMultiplier(ThisPtr->length);
        // encodeBits((byte) multiplier, 6);
        // return multiplier * 16384l;
        stream->error.error = ASN_ERROR_SIZE_NOT_SUPPORTED;
        return &stream->error;
    }

    if ((U32)ASN_Stream_BitsLeft(stream) < ThisPtr->length * 8) {
        stream->error.error = ASN_ERROR_BUFFER_FULL;
        return &stream->error;
    }

    for (i = 0; i < ThisPtr->length; i++) {
        result = ASN_Stream_EncodeByte(stream, ThisPtr->data[i]);
        if (result != ASN_RESULT_OK) {
            return result;
        }
    }
    return ASN_RESULT_OK;
}

U32 ASN_UTF8String_EncodedSize(ASN_UTF8String ThisPtr, U32 low, U32 high) {
    U32 LengthDeterminatorSizeInBits = 0u;
    if (ThisPtr->length < 128) {
        LengthDeterminatorSizeInBits = 8;
    } else if (ThisPtr->length < 16384) {
        LengthDeterminatorSizeInBits = 16;
    }

    return LengthDeterminatorSizeInBits + ThisPtr->length * 8;
}

U32 ASN_UTF8String_Get(ASN_UTF8String ThisPtr, ASN_BYTE** data) {
    return ASN_OctetString_Get((ASN_OctetString)ThisPtr, data);
}

char* ASN_UTF8String_GetText(ASN_UTF8String ThisPtr) { return ASN_OctetString_GetText((ASN_OctetString)ThisPtr); }

#ifdef ENABLE_ASN_SESSION_SIZE
U32 ASN_UTF8String_SessionSize(U32 low, U32 high) { return ASN_OctetString_SessionSize(low, high); }
#endif /* #ifdef ENABLE_ASN_SESSION_SIZE */

ASN_Result ASN_UTF8String_Set(ASN_UTF8String ThisPtr, ASN_Session session, ASN_BYTE* data, U32 length) {
    return ASN_OctetString_Set((ASN_OctetString)ThisPtr, session, data, length);
}

ASN_Result ASN_UTF8String_SetText(ASN_UTF8String ThisPtr, ASN_Session session, char* data) {
    return ASN_OctetString_SetText((ASN_OctetString)ThisPtr, session, data);
}

ASN_Result BOOL_Decode(BOOL* value, ASN_Session session, ASN_Stream stream) {
    UNUSED_VARIABLE(session);
    if (ASN_Stream_BitsLeft(stream) < 1) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }
    *value = (BOOL)ASN_Stream_DecodeBits(stream, 1);
    return ASN_RESULT_OK;
}

ASN_Result BOOL_Encode(BOOL value, ASN_Stream stream) {
    // value == TRUE, is needed otherwise someone might send a value like H'xxxxxxx1
    // This would cause the value true to be encoded, however if this is the bool for an optional parameter
    // that parameter will not be encode, causing a mismatch between encoded and decoded data.
    return ASN_Stream_EncodeBits(stream, (ASN_BYTE)(value == TRUE), 1);
}

U32 BOOL_EncodedSize(BOOL a) {
    UNUSED_VARIABLE(a);
    return 1u;
}

ASN_Result ENUM_Decode(U32* value, ASN_Session session, ASN_Stream stream, U32 low, U32 maxEnumValue, BOOL extensions) {
    // X.691 Section 13
    UNUSED_VARIABLE(session);
    UNUSED_VARIABLE(low);
    if (extensions == TRUE) {
        // 13.3
        BOOL extended;
        if (BOOL_Decode(&extended, 0, stream) != ASN_RESULT_OK) {
            return &stream->error;
        }

        if (extended == TRUE) {
            if (ASN_SmallUnsigned_Decode(stream, value) != ASN_RESULT_OK) {
                return &stream->error;
            }
            *value += maxEnumValue + 1;
            return ASN_RESULT_OK;
        }
    }

    // 13.2
    return U32_Decode(value, 0, stream, 0, maxEnumValue);
}

ASN_Result ENUM_Encode(U32 value, ASN_Stream stream, U32 low, U32 maxEnumValue, BOOL extensions) {
    // X.691 Section 13
    UNUSED_VARIABLE(low);
    if (extensions == TRUE) {
        // 13.3
        BOOL extended = value > maxEnumValue;
        BOOL_Encode(extended, stream);
        if (extended == TRUE) {
            return ASN_SmallUnsigned_Encode(stream, value - maxEnumValue - 1);
        }
    }

    // 13.2
    return U32_Encode(value, stream, 0, maxEnumValue);
}

U32 ENUM_EncodedSize(U32 value, U32 low, U32 maxEnumValue, BOOL extensions) {
    if (extensions == TRUE) {
        if (value > maxEnumValue) {
            return 1 + ASN_SmallUnsigned_EncodedSize(value);
        } else {
            return 1 + ASN_U32CountBits(low, maxEnumValue);
        }
    }
    return ASN_U32CountBits(low, maxEnumValue);
}
U32 ENUM_SessionSize(U32 low, U32 high, BOOL extensions) {
    UNUSED_VARIABLE(low);
    UNUSED_VARIABLE(high);
    UNUSED_VARIABLE(extensions);
    return 0u;  // This type is stored in the structure
}

ASN_Result S32_Decode(S32* value, ASN_Session session, ASN_Stream stream, S32 low, S32 high) {
    U32 bitCount = ASN_S32CountBits(low, high);
    UNUSED_VARIABLE(session);

    if (ASN_Stream_BitsLeft(stream) < bitCount) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }
    *value = low + ASN_Stream_DecodeInt(stream, bitCount);

    if (*value < low || *value > high) {
        stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return &stream->error;
    } else
        return ASN_RESULT_OK;
}

ASN_Result S32_Encode(S32 value, ASN_Stream stream, S32 low, S32 high) {
    if (value < low || value > high) {
        /* The line-number will be set in the generated code */
        stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return &stream->error;
    } else
        return ASN_Stream_EncodeInt(stream, (U32)(value - low), ASN_S32CountBits(low, high));
}

U32 S32_EncodedSize(S32 a, S32 low, S32 high) {
    UNUSED_VARIABLE(a);
    return ASN_S32CountBits(low, high);
}

// The memory for these primitive types are allocated with the struct itself
U32 S32_SessionSize(S32 a, S32 b) {
    UNUSED_VARIABLE(a);
    UNUSED_VARIABLE(b);
    return 0u;
}

ASN_Result U32_Decode(U32* value, ASN_Session session, ASN_Stream stream, U32 low, U32 high) {
    U32 bitCount = ASN_U32CountBits(low, high);
    UNUSED_VARIABLE(session);

    if (ASN_Stream_BitsLeft(stream) < bitCount) {
        stream->error.error = ASN_ERROR_BUFFER_STARVING;
        return &stream->error;
    }
    *value = ASN_Stream_DecodeInt(stream, bitCount) + low;
    if (*value < low || *value > high) {
        stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return &stream->error;
    } else
        return ASN_RESULT_OK;
}

ASN_Result U32_Encode(U32 value, ASN_Stream stream, U32 low, U32 high) {
    if (value < low || value > high) {
        /* The line-number will be set in the generated code */
        stream->error.error = ASN_ERROR_VALUE_NOT_WITHIN_RANGE;
        return &stream->error;
    } else
        return ASN_Stream_EncodeInt(stream, value - low, ASN_U32CountBits(low, high));
}

U32 U32_EncodedSize(U32 a, U32 low, U32 high) {
    UNUSED_VARIABLE(a);
    return ASN_U32CountBits(low, high);
}

// The memory for these primitive types are allocated with the struct itself
U32 U32_SessionSize(U32 a, U32 b) {
    UNUSED_VARIABLE(a);
    UNUSED_VARIABLE(b);
    return 0u;
}

#ifdef ASN_PRINT

void ASN_ObjectId_Print(ASN_ObjectId ThisPtr) {
    U32 i;
    ASN_PrintIndented("0x");
    for (i = 0; i < ThisPtr->length; i++) {
        ASN_Print("%02X", (U32)ThisPtr->data[i]);
    }
    ASN_Print(EOL);
}

void ASN_OctetString_Print(ASN_OctetString ThisPtr) { ASN_String_Print((struct ASN_String_t*)ThisPtr); }

void ASN_PrintableString_Print(ASN_PrintableString ThisPtr) {
    U32 i;
    struct ASN_String_t tString;
    tString.data = ThisPtr->data;
    tString.length = ThisPtr->length;
    ASN_String_Print(&tString);
}

void ASN_UTF8String_Print(ASN_UTF8String ThisPtr) { ASN_String_Print((struct ASN_String_t*)ThisPtr); }

#endif /* #ifdef ASN_PRINT */

// Testcode
#ifdef ASN_ENABLE_TESTCODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static U32 HexToByte(char* string, ASN_BYTE* binary);
static void ByteToHex(ASN_BYTE* binary, U32 length, char* string);

static BOOL assertEqualsU32(U32 should_be, U32 is, U32 line_number) {
    if (should_be == is)
        return true;
    else {
        ASN_Print("%s:%d, Expected: %u, was: %u" EOL, __FILE__, line_number, should_be, is);
        return false;
    }
}

static BOOL assertNotEqualsPtr(void* should_not_be, void* is, U32 line_number) {
    if (should_not_be != is)
        return true;
    else {
        ASN_Print("%s:%d, Should not be: %u" EOL, __FILE__, line_number, should_not_be, is);
        return false;
    }
}

#define assertEqualsPtr(a, b, c) assertEqualsU32((U32)a, (U32)b, c)

static BOOL assertString(char* should_be, char* is, U32 line_number) {
    U32 result = strcmp(should_be, is);
    if (result == 0)
        return true;
    else {
        ASN_Print("%s:%d" EOL "Expected: %s\nGot     : %s" EOL, __FILE__, line_number, should_be, is);
        return false;
    }
}

void test_S32CountBits(void) {
    assertEqualsU32(1, ASN_S32CountBits(1, 2), __LINE__);
    assertEqualsU32(1, ASN_S32CountBits(0, 1), __LINE__);
    assertEqualsU32(1, ASN_S32CountBits(-1, 0), __LINE__);
    assertEqualsU32(1, ASN_S32CountBits(-2, -1), __LINE__);
    assertEqualsU32(31, ASN_S32CountBits(-2147483647 - 1, -1), __LINE__);
    assertEqualsU32(31, ASN_S32CountBits(-2147483647, 0), __LINE__);
    assertEqualsU32(32, ASN_S32CountBits(-2147483647 - 1, 0), __LINE__);
    assertEqualsU32(32, ASN_S32CountBits(-2147483647 - 1, 1), __LINE__);
    assertEqualsU32(32, ASN_S32CountBits(-1, 2147483647), __LINE__);
    assertEqualsU32(31, ASN_S32CountBits(1, 2147483647), __LINE__);
    assertEqualsU32(32, ASN_S32CountBits(-2147483647, 2147483647), __LINE__);
    assertEqualsU32(8, ASN_S32CountBits(-128, 127), __LINE__);
    assertEqualsU32(8, ASN_S32CountBits(-256, -128), __LINE__);
}

void test_Stream() { assertEqualsU32(sizeof(struct ASN_Stream_t), ASN_STREAM_SIZE, __LINE__); }

void test_StreamDecodeByte() {
    void* encode_buffer = malloc(ASN_STREAM_SIZE);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, 200);

    char* sMessage =
            "251C80434090000010288FC08A323232323232323232323232323232323006B3122A7FFAC10BF618CE6DCEC000007A00001002085F"
            "C9FE73610606C0B63844043E1042FE4DB39B0C406513AF812001F08217F27B9CD89B83588D5C69028F8410BF8CB4E6C58E0680A1B0"
            "02547C2085FC5AE7362FA0480B500F8B27E0C09217FF9D20002A6C3C00";
    ASN_BYTE* byMsg = malloc(strlen(sMessage) / 2);
    U32 iBufferSize = HexToByte(sMessage, byMsg);

    ASN_Stream_AttachBuffer(stream, byMsg, iBufferSize);

    assertEqualsU32(0x25, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x1C, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x80, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x43, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x40, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x90, ASN_Stream_DecodeByte(stream), __LINE__);
    free(byMsg);
    free(encode_buffer);
}

void test_StreamReset() {
    void* encode_buffer = malloc(ASN_STREAM_SIZE);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, 200);

    char* sMessage =
            "251C80434090000010288FC08A323232323232323232323232323232323006B3122A7FFAC10BF618CE6DCEC000007A00001002085F"
            "C9FE73610606C0B63844043E1042FE4DB39B0C406513AF812001F08217F27B9CD89B83588D5C69028F8410BF8CB4E6C58E0680A1B0"
            "02547C2085FC5AE7362FA0480B500F8B27E0C09217FF9D20002A6C3C00";
    ASN_BYTE* byMsg = malloc(strlen(sMessage) / 2);
    U32 iBufferSize = HexToByte(sMessage, byMsg);

    ASN_Stream_AttachBuffer(stream, byMsg, iBufferSize);

    assertEqualsU32(0x25, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x1C, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x80, ASN_Stream_DecodeByte(stream), __LINE__);
    ASN_Stream_Reset(stream);
    assertEqualsU32(0x25, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x1C, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0x80, ASN_Stream_DecodeByte(stream), __LINE__);
    free(byMsg);
    free(encode_buffer);
}

void test_StreamBitsLeft() {
    void* encode_buffer = malloc(ASN_STREAM_SIZE);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, 200);

    char* sMessage = "251C80";
    ASN_BYTE* byMsg = malloc(strlen(sMessage) / 2);
    U32 iBufferSize = HexToByte(sMessage, byMsg);

    ASN_Stream_AttachBuffer(stream, byMsg, iBufferSize);

    assertEqualsU32(24, ASN_Stream_BitsLeft(stream), __LINE__);
    assertEqualsU32(0x25, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(16, ASN_Stream_BitsLeft(stream), __LINE__);
    assertEqualsU32(0x1C, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(8, ASN_Stream_BitsLeft(stream), __LINE__);
    assertEqualsU32(0x80, ASN_Stream_DecodeByte(stream), __LINE__);
    assertEqualsU32(0, ASN_Stream_BitsLeft(stream), __LINE__);
    free(byMsg);
    free(encode_buffer);
}

U32 ASN_Stream_DecodeInt(ASN_Stream stream, U32 bitCount);

void test_StreamDecodeInt() {
    void* encode_buffer = malloc(ASN_STREAM_SIZE);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, 200);

    char* sMessage =
            "251C80434090000010288FC08A323232323232323232323232323232323006B3122A7FFAC10BF618CE6DCEC000007A00001002085F"
            "C9FE73610606C0B63844043E1042FE4DB39B0C406513AF812001F08217F27B9CD89B83588D5C69028F8410BF8CB4E6C58E0680A1B0"
            "02547C2085FC5AE7362FA0480B500F8B27E0C09217FF9D20002A6C3C00";
    ASN_BYTE* byMsg = malloc(strlen(sMessage) / 2);
    U32 iBufferSize = HexToByte(sMessage, byMsg);

    ASN_Stream_AttachBuffer(stream, byMsg, iBufferSize);

    assertEqualsU32(0x251C, ASN_Stream_DecodeInt(stream, 16), __LINE__);
    assertEqualsU32(0x80, ASN_Stream_DecodeInt(stream, 8), __LINE__);
    assertEqualsU32(0x434090, ASN_Stream_DecodeInt(stream, 24), __LINE__);
    assertEqualsU32(0x00001028, ASN_Stream_DecodeInt(stream, 32), __LINE__);
    assertEqualsU32(0x8FC08A32, ASN_Stream_DecodeInt(stream, 32), __LINE__);
    free(byMsg);
    free(encode_buffer);
}

void test_U32Decode() {
    void* encode_buffer = malloc(ASN_STREAM_SIZE);
    void* decode_buffer = malloc(300);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, ASN_STREAM_SIZE);
    ASN_Session session = ASN_Session_Create(decode_buffer, 300);

    char* sMessage = "FF1C80434090000010288FC08A3232";
    ASN_BYTE* byMsg = malloc(strlen(sMessage) / 2);
    U32 iBufferSize = HexToByte(sMessage, byMsg);
    U32 value;
    ASN_Result result;

    ASN_Stream_AttachBuffer(stream, byMsg, iBufferSize);

    result = U32_Decode(&value, session, stream, 0, 40000);
    assertNotEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    /* Check that we have ASN_ERROR_VALUE_NOT_WITHIN_RANGE */
    assertEqualsU32(ASN_ERROR_VALUE_NOT_WITHIN_RANGE, result->error, __LINE__);
    assertEqualsU32(0xFF1C, value, __LINE__);

    result = U32_Decode(&value, session, stream, 0, 4294967295);
    if (!assertEqualsPtr(ASN_RESULT_OK, result, __LINE__)) {
        ASN_Print("Error code: %d, at line: %d" EOL, (int)result->error, (int)result->linenumber);
    }
    assertEqualsU32(0x80434090, value, __LINE__);

    result = U32_Decode(&value, session, stream, 0, 4294967295);
    if (!assertEqualsPtr(ASN_RESULT_OK, result, __LINE__)) {
        ASN_Print("Error code: %d, at line: %d" EOL, (int)result->error, (int)result->linenumber);
    }
    assertEqualsU32(0x00001028, value, __LINE__);

    result = U32_Decode(&value, session, stream, 0, 4294967295);
    if (!assertEqualsPtr(ASN_RESULT_OK, result, __LINE__)) {
        ASN_Print("Error code: %d, at line: %d" EOL, (int)result->error, (int)result->linenumber);
    }
    assertEqualsU32(0x8FC08A32, value, __LINE__);

    result = U32_Decode(&value, session, stream, 3, 250);
    if (!assertEqualsPtr(ASN_RESULT_OK, result, __LINE__)) {
        ASN_Print("Error code: %d, at line: %d" EOL, (int)result->error, (int)result->linenumber);
    }
    assertEqualsU32(0x35, value, __LINE__);

    result = U32_Decode(&value, session, stream, 0, 255);
    assertNotEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    /* Check that we have ASN_ERROR_BUFFER_STARVING */
    assertEqualsU32(ASN_ERROR_BUFFER_STARVING, result->error, __LINE__);

    free(byMsg);
    free(decode_buffer);
    free(encode_buffer);
}

void test_U32Encode(void) {
    ASN_BYTE encode_buffer[ASN_STREAM_SIZE + 6];
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, sizeof(encode_buffer));
    ASN_BYTE* encoded_buffer;
    U32 encoded_length;
    char result_string[21];
    ASN_Result result;
    assertEqualsU32(6 * 8, ASN_Stream_BitsLeft(stream), __LINE__);

    assertNotEqualsPtr(NULL, stream, __LINE__);

    result = U32_Encode(0xFF, stream, 0, 0x80);
    assertNotEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    /* Check that we have ASN_ERROR_VALUE_NOT_WITHIN_RANGE */
    assertEqualsU32(ASN_ERROR_VALUE_NOT_WITHIN_RANGE, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(0, encoded_length, __LINE__);

    result = U32_Encode(0xED, stream, 0, 0xFF);
    assertEqualsU32(ASN_RESULT_OK, (U32)result, __LINE__);

    result = U32_Encode(0xFAEDFAED, stream, 0, 0xFFFFFFFF);
    assertEqualsU32(ASN_RESULT_OK, (U32)result, __LINE__);

    result = U32_Encode(0, stream, 0, 1);
    assertEqualsU32(ASN_RESULT_OK, (U32)result, __LINE__);

    result = U32_Encode(2, stream, 0, 3);
    assertEqualsU32(ASN_RESULT_OK, (U32)result, __LINE__);

    result = U32_Encode(0xFAEDFAED, stream, 0, 0xFFFFFFFF);
    assertNotEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    /* Check that we have ASN_ERROR_BUFFER_FULL */
    assertEqualsU32(ASN_ERROR_BUFFER_FULL, result->error, __LINE__);

    assertEqualsU32(32 + 8 + 1 + 2, ASN_Stream_BitsUsed(stream), __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(6, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("EDFAEDFAED40", result_string, __LINE__);
}

void test_EncodeByte(void) {
    void* encode_buffer = malloc(ASN_STREAM_SIZE + 10);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, ASN_STREAM_SIZE + 10);
    ASN_BYTE* encoded_buffer;
    U32 encoded_length;
    char result_string[21];
    ASN_Result result;

    assertEqualsU32(10 * 8, ASN_Stream_BitsLeft(stream), __LINE__);

    ASN_Stream_EncodeByte(stream, 0x25);
    assertEqualsU32(9 * 8, ASN_Stream_BitsLeft(stream), __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(1, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25", result_string, __LINE__);

    ASN_Stream_EncodeByte(stream, 0xFF);
    ASN_Stream_EncodeByte(stream, 0xAC);
    ASN_Stream_EncodeByte(stream, 0x12);
    ASN_Stream_EncodeByte(stream, 0x43);
    ASN_Stream_EncodeByte(stream, 0x99);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(6, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25FFAC124399", result_string, __LINE__);

    ASN_Stream_EncodeByte(stream, 0xAC);
    ASN_Stream_EncodeByte(stream, 0x12);
    ASN_Stream_EncodeByte(stream, 0x43);
    ASN_Stream_EncodeByte(stream, 0x99);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(10, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25FFAC124399AC124399", result_string, __LINE__);

    /* Should be buffer full */
    result = ASN_Stream_EncodeByte(stream, 0x99);
    assertNotEqualsPtr(NULL, result, __LINE__);
    assertEqualsU32(ASN_ERROR_BUFFER_FULL, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(10, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25FFAC124399AC124399", result_string, __LINE__);

    free(encode_buffer);
}

void test_EncodeBits(void) {
    void* encode_buffer = malloc(ASN_STREAM_SIZE + 10);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, ASN_STREAM_SIZE + 10);
    ASN_BYTE* encoded_buffer;
    U32 encoded_length;
    char result_string[21];
    ASN_Result result;

    assertEqualsU32(10 * 8, ASN_Stream_BitsLeft(stream), __LINE__);

    ASN_Stream_EncodeBits(stream, 0x2, 4);
    ASN_Stream_EncodeBits(stream, 0x5, 4);
    assertEqualsU32(9 * 8, ASN_Stream_BitsLeft(stream), __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(1, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25", result_string, __LINE__);

    ASN_Stream_EncodeBits(stream, 0x2, 2);
    ASN_Stream_EncodeBits(stream, 0x3, 2);
    ASN_Stream_EncodeBits(stream, 0x0, 2);
    ASN_Stream_EncodeBits(stream, 0x3, 2);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(2, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25B3", result_string, __LINE__);

    ASN_Stream_EncodeBits(stream, 0x1, 1);
    ASN_Stream_EncodeBits(stream, 0x0, 1);
    ASN_Stream_EncodeBits(stream, 0xE, 4);
    ASN_Stream_EncodeBits(stream, 0x37, 6);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(4, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25B3BB70", result_string, __LINE__);

    ASN_Stream_EncodeBits(stream, 0x3, 4);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(4, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25B3BB73", result_string, __LINE__);

    ASN_Stream_EncodeBits(stream, 0xef, 8);
    ASN_Stream_EncodeBits(stream, 0xfe, 8);
    ASN_Stream_EncodeBits(stream, 0xef, 8);
    ASN_Stream_EncodeBits(stream, 0xfe, 8);
    ASN_Stream_EncodeBits(stream, 0xef, 8);
    ASN_Stream_EncodeBits(stream, 0x7a, 7);

    /* Should NOT work */
    result = ASN_Stream_EncodeBits(stream, 0x3, 2);
    assertNotEqualsPtr(NULL, result, __LINE__);
    if (result != NULL) assertEqualsU32(ASN_ERROR_BUFFER_FULL, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(10, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25B3BB73EFFEEFFEEFF4", result_string, __LINE__);

    result = ASN_Stream_EncodeBits(stream, 0x1, 1);
    assertEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    if (result != ASN_RESULT_OK) assertEqualsU32(0, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(10, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("25B3BB73EFFEEFFEEFF5", result_string, __LINE__);

    free(encode_buffer);
}

void test_EncodeInt(void) {
    void* encode_buffer = malloc(ASN_STREAM_SIZE + 10);
    ASN_Stream stream = ASN_Stream_Create(encode_buffer, ASN_STREAM_SIZE + 10);
    ASN_BYTE* encoded_buffer;
    U32 encoded_length;
    char result_string[21];
    ASN_Result result;

    assertEqualsU32(10 * 8, ASN_Stream_BitsLeft(stream), __LINE__);

    result = ASN_Stream_EncodeInt(stream, 0x77, ASN_U32CountBits(0, 255));
    assertEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    if (result != ASN_RESULT_OK) assertEqualsU32(0, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(1, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("77", result_string, __LINE__);

    result = ASN_Stream_EncodeInt(stream, 0x99887766, 32);
    assertEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    if (result != ASN_RESULT_OK) assertEqualsU32(0, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(5, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("7799887766", result_string, __LINE__);

    result = ASN_Stream_EncodeInt(stream, 0x99887766, 16);
    assertNotEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    if (result != ASN_RESULT_OK) assertEqualsU32(ASN_ERROR_VALUE_NOT_WITHIN_RANGE, result->error, __LINE__);

    result = ASN_Stream_EncodeInt(stream, 0x99887766, 33);
    assertNotEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    if (result != ASN_RESULT_OK) assertEqualsU32(ASN_ERROR_SIZE_NOT_SUPPORTED, result->error, __LINE__);

    result = ASN_Stream_EncodeInt(stream, 0x9988FF, 24);
    assertEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    if (result != ASN_RESULT_OK) assertEqualsU32(0, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(8, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("77998877669988FF", result_string, __LINE__);

    result = ASN_Stream_EncodeInt(stream, 0xffbb4455, 32);
    assertNotEqualsPtr(ASN_RESULT_OK, result, __LINE__);
    if (result != ASN_RESULT_OK) assertEqualsU32(ASN_ERROR_BUFFER_FULL, result->error, __LINE__);

    encoded_buffer = ASN_Stream_GetBuffer(stream, &encoded_length);
    assertNotEqualsPtr(NULL, encoded_buffer, __LINE__);
    assertEqualsU32(10, encoded_length, __LINE__);

    ByteToHex(encoded_buffer, encoded_length, result_string);
    assertString("77998877669988FFFFBB", result_string, __LINE__);

    free(encode_buffer);
}

#define ASN_SESSION_TEST_SIZE 2000

void testSession(void) {
    void* decode_buffer = malloc(ASN_SESSION_SIZE + ASN_SESSION_TEST_SIZE);
    ASN_Session session = ASN_Session_Create(decode_buffer, ASN_SESSION_SIZE + ASN_SESSION_TEST_SIZE);
    void* test;
    void* test2;
    void* test3;

    assertEqualsU32(sizeof(struct ASN_Session_t), ASN_SESSION_SIZE, __LINE__);

    assertEqualsU32(ASN_SESSION_TEST_SIZE, ASN_Session_BytesLeft(session), __LINE__);

    test = ASN_Malloc(session, 3);
    assertNotEqualsPtr(NULL, test, __LINE__);

    test2 = ASN_Malloc(session, 1910);
    assertNotEqualsPtr(NULL, test2, __LINE__);

    assertEqualsU32(ASN_SESSION_TEST_SIZE - 3 - 1910, ASN_Session_BytesLeft(session), __LINE__);

    test3 = ASN_Malloc(session, 100);
    assertEqualsU32((U32)NULL, (U32)test3, __LINE__);
    assertEqualsU32(ASN_SESSION_TEST_SIZE - 3 - 1910, ASN_Session_BytesLeft(session), __LINE__);

    ASN_Session_Reset(session);
    assertEqualsU32(ASN_SESSION_TEST_SIZE, ASN_Session_BytesLeft(session), __LINE__);

    free(decode_buffer);
}

static int charToNibble(char c) {
    if ('0' <= c && c <= '9') {
        return c - '0';
    } else if ('a' <= c && c <= 'f') {
        return c - 'a' + 0xa;
    } else if ('A' <= c && c <= 'F') {
        return c - 'A' + 0xa;
    } else {
        ASN_Print("Invalid hex character: %c" EOL, c);
        return 0;
    }
}

static U32 HexToByte(char* string, ASN_BYTE* binary) {
    U32 stringLength = strlen(string);
    U32 i = 0, j = 0;

    if ((stringLength & 0x1) != 0) {
        ASN_Print("Hex string must be in multiple of two" EOL);
        return 0;
    }
    for (; i < stringLength; i += 2, j++) {
        U32 high = charToNibble(string[i]);
        U32 low = charToNibble(string[i + 1]);
        binary[j] = (ASN_BYTE)((high << 4) | low);
    }

    return stringLength / 2;
}

static void ByteToHex(ASN_BYTE* binary, U32 length, char* string) {
    U32 i;

    for (i = 0; i < length; i++) {
        sprintf(string + 2 * i, "%02X", (int)binary[i]);
    }
    string[2 * length + 1] = '\0';
}

void ASN_TestCode(void) {
    ASN_Print("ASN tests starts" EOL);
    test_Stream();
    test_S32CountBits();
    test_StreamDecodeByte();
    test_StreamReset();
    test_StreamBitsLeft();
    test_StreamDecodeInt();
    test_U32Decode();
    test_U32Encode();
    testSession();

    test_EncodeByte();
    test_EncodeBits();
    test_EncodeInt();

    ASN_Print("ASN tests ends" EOL "There should be no printouts between \"start\" and \"end\"" EOL);
}

#endif  /// #ifdef ASN_ENABLE_TESTCODE

/// @}
