#ifndef PL_ASN_BASE_H
#define PL_ASN_BASE_H 1
/** @defgroup PL_ASN_BASE ASN.1 Interface
*** @ingroup PL
*** @{
***
*** Copyright (C) 2012 ACTIA NORDIC AB. All rights reserved
***
*** @par Project:
*** TEM3
***
*** @par Module:
*** PL/ASN_BASE ASN.1 Interface
***
*** @par Purpose:
*** Define an interface for primitive functions of ASN.1
***
*** @par Usage Notes:
*** None.
***/
#include "tem3_interface/tem3_basic_types.h"

// --------------------------------------------------------------------------
// Definitions.
#ifndef EOL
#define EOL "\r\n"
#endif

#ifndef FALSE
#define FALSE (BOOL)0
#endif

#ifndef TRUE
#define TRUE (BOOL)1
#endif

#define ASN_Null		void *

#define ASN_RESULT_OK	0

//#define ASN_PER_ALIGN   1
#define ASN_PER_ALIGN   0
#define ASN_ALIGNMENT   4

typedef unsigned char	ASN_BYTE;
typedef bool			   BOOL;
typedef U8				   ASN_Errcode;

typedef struct ASN_Result_t
{
   ASN_Errcode	error;
   U32			linenumber;
} * ASN_Result;

#define ASN_SESSION_SIZE (3*sizeof(ASN_BYTE*)+sizeof(U32)+sizeof(struct ASN_Result_t))
#define ASN_STREAM_SIZE (2*sizeof(ASN_BYTE*)+2*sizeof(U32)+sizeof(struct ASN_Result_t))

#ifdef ASN_ALIGNMENT
#define _ASN_ALIGN(SIZE,W) SIZE % W? ((SIZE / W ) + 1 ) * W: SIZE
#define ASN_ALIGN(SIZE) _ASN_ALIGN(SIZE,ASN_ALIGNMENT)
#endif

#define ASN_DECODE_CHECK(fn) result = fn; if ( result != (ASN_Result)ASN_RESULT_OK ) { if (result->linenumber == 0) result->linenumber = __LINE__; return result; }
#define ASN_ENCODE_CHECK(fn) result = fn; if ( result != (ASN_Result)ASN_RESULT_OK ) { if (result->linenumber == 0) result->linenumber = __LINE__; return result; }
#define ASN_SETSIZE_CHECK(fn) result = fn; if ( result != (ASN_Result)ASN_RESULT_OK ) { if (result->linenumber == 0) result->linenumber = __LINE__; return result; }
#define ASN_SETTYPE_CHECK(fn) result = fn; if ( result != (ASN_Result)ASN_RESULT_OK ) { if (result->linenumber == 0) result->linenumber = __LINE__; return result; }
#define ASN_MEM_CHECK(v) if ( v == NULL ) { result = ASN_Session_GetResult( session ); result->error = ASN_ERROR_MEMORY; result->linenumber = __LINE__; return result; }

typedef struct ASN_Session_t * ASN_Session;
typedef struct ASN_ObjectId_t * ASN_ObjectId;
typedef struct ASN_ObjectId_t * ASN_RelativeObjectId;
typedef struct ASN_String_t * ASN_OctetString;
typedef struct ASN_PrintableString_t * ASN_PrintableString;
typedef struct ASN_String_t * ASN_UTF8String;
typedef struct ASN_Stream_t * ASN_Stream;

/// The memory for these primitive types are allocated with the struct itself
#define ASN_Null_SessionSize() (0)
#define ASN_Null_EncodedSize(a) (0)
#define ASN_Null_Decode(a,b,c) (ASN_RESULT_OK)
#define ASN_Null_Encode(a,b) (ASN_RESULT_OK)

// --------------------------------------------------------------------------
// Type Declarations.
enum {
   ASN_ERROR_NO_ERROR,
   ASN_ERROR_MEMORY,
   ASN_ERROR_BUFFER_FULL,
   ASN_ERROR_BUFFER_STARVING,
   ASN_ERROR_NO_VALID_CHOICE,
   ASN_ERROR_VALUE_NOT_WITHIN_RANGE,
   ASN_ERROR_SIZE_NOT_SUPPORTED
};

// --------------------------------------------------------------------------
// Exported Variable Declarations.
// --------------------------------------------------------------------------
// Exported Function Declarations.

/** Decode a length-encode value from stream
***
*** @par Purpose:
*** Decode length variable. It uses the low and high values to determine
*** number of bits for storing length variable on stream.
***
*** @param  stream The stream handle.
***
*** @param  length  U32 pointer for storing decoded length within.
***
*** @param  low The min value for the length.
***
*** @param  high The max value for the length.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***/
ASN_Result ASN_Length_Decode( ASN_Stream stream, U32 *length, U32 low, U32 high);

/** Encode a length on stream
***
*** @par Purpose:
*** Encode length variable. It uses the low and high values to determine
*** number of bits for storing length variable on stream.
***
*** @param  stream The stream handle.
***
*** @param  length  The value to encode with length-encoding.
***
*** @param  low The min value for the length.
***
*** @param  high The max value for the length.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***/
ASN_Result ASN_Length_Encode( ASN_Stream stream, U32 length, U32 low, U32 high);

/** Allocates memory from session buffer
***
*** @par Purpose:
*** Retreives memory from within the session buffer.
*** layered protocol demands byte alignment between layers.
***
*** @param  session The session handle.
***
*** @param  size The size of the needed memory block.
***
*** @return Returns a pointer to a memory block of the requested size or
*** NULL if not successful. The only fault-case is lack of memory
*** in the session.
***
***/
void * ASN_Malloc(  ASN_Session session, U32 size );

/** Decode a multibyte
***
*** @par Purpose:
*** Decode a multibyte.
***
*** @param  stream The stream handle.
***
*** @param  value Pointer to store a decoded multibyte in.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
*** in the generated code.
***
***/
ASN_Result ASN_MultiByte_Decode( ASN_Stream stream, U32 *value);

/** Encode a multibyte
***
*** @par Purpose:
*** Encode a multibyte.
***
*** @param  stream The stream handle.
***
*** @param  value Value to be encoded as a multibyte.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
*** in the generated code.
***
***/
ASN_Result ASN_MultiByte_Encode( ASN_Stream stream, U32 value);

/** Get size in bits to store a multibyte in ASN_Stream
***
*** @par Purpose:
*** Determine the number of bits that is required for storing an encoded
*** value as a multibyte.
***
*** @param  value Value to be encoded as a multibyte.
***
*** @return Number of bits needed for encoding value as a multibyte.
***
***/
U32 ASN_MultiByte_EncodedSize( U32 value);

/** Associate a U16 array with ASN_ObjectId
***
*** @par Purpose:
*** Used to be able to encode a U16 array from it's original place.
***
*** @param  ThisPtr A string handle.
***
*** @param  data Pointer to pre-allocated 16-bit data.
***
*** @param  The number of elements (arcs) of this 16-bit array..
***
***/
void ASN_ObjectId_Associate( ASN_ObjectId ThisPtr, U16 * data, U16 arcs );

/** Create an ASN_ObjectId
***
*** @par Purpose:
*** Used by the generated code where an "OBJECT IDENTIFIER" is declared in ASN.1.
***
*** @param  session Session handle.
***
*** @return A pointer to the newly created string or NULL if 
***         there where not enough memory in the session.
***         The generated code will propagate the "right" error.
***
***/
ASN_ObjectId ASN_ObjectId_Create( ASN_Session session );

/** Decode an ASN_ObjectId
***
*** @par Purpose:
*** Used by the generated code where an "OBJECT IDENTIFIER" is declared in ASN.1.
***
*** @param  ThisPtr ASN_ObjectId handle.
***
*** @param  session Session handle.
***
*** @param  stream Stream handle.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_ObjectId_Decode( ASN_ObjectId ThisPtr, ASN_Session session, ASN_Stream stream );

/** Encode an ASN_ObjectId
***
*** @par Purpose:
*** Used by the generated code where an "OBJECT IDENTIFIER" is declared in ASN.1.
***
*** @param  ThisPtr ASN_ObjectId handle.
***
*** @param  stream Stream handle.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_ObjectId_Encode( ASN_ObjectId ThisPtr, ASN_Stream stream );

/** Get size in bits to store a encoded ASN_ObjectId
***
*** @par Purpose:
*** Used by the generated code where an "OBJECT IDENTIFIER" is declared in ASN.1.
***
*** @param  ThisPtr ASN_ObjectId handle.
***
*** @return The size in bits needed for encoding.
***
***/
U32 ASN_ObjectId_EncodedSize( ASN_ObjectId ThisPtr );


#ifdef ENABLE_ASN_SESSION_SIZE
/** Get size in bytes needed to store an ASN_ObjectId in a session
***
*** @par Purpose:
*** Used by the generated code where an "OBJECT IDENTIFIER" is declared in ASN.1.
***
*** @return The size in bytes needed to decode this string into a session.
***
***/
U32 ASN_ObjectId_SessionSize(  );
#endif /* #ifdef ENABLE_ASN_SESSION_SIZE */

/** Associate a byte array with string
***
*** @par Purpose:
*** Used to be able to encode a byte array from it's original place.
***
*** @param  ThisPtr A string handle.
***
*** @param  data Pointer to pre-allocated data.
***
*** @param  length String length.
***
***/
void ASN_OctetString_Associate( ASN_OctetString ThisPtr, ASN_BYTE * data, U32 length );

/** Associate a null-terminated array with string
***
*** @par Purpose:
*** Used to be able to encode a null-terminated string from it's original place.
***
*** @param  ThisPtr String handle.
***
*** @param  text Pointer to a null-terminated string
***
***/
void ASN_OctetString_AssociateText( ASN_OctetString ThisPtr, char * text );

/** Create an ASN_OctetString
***
*** @par Purpose:
*** Used by the generated code where an "OCTET STRING" is declared in ASN.1.
***
*** @param  session A string handle.
***
*** @return A pointer to the newly created string or NULL if 
***         there where not enough memory in the session.
***         The generated code will propagate the "right" error.
***
***/
ASN_OctetString ASN_OctetString_Create( ASN_Session session );

/** Decode an ASN_OctetString
***
*** @par Purpose:
*** Used by the generated code where an "OCTET STRING" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  session A session handle.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_OctetString_Decode( ASN_OctetString ThisPtr, ASN_Session session, ASN_Stream stream, U32 low, U32 high );

/** Encode an ASN_OctetString
***
*** @par Purpose:
*** Used by the generated code where an "OCTET STRING" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_OctetString_Encode( ASN_OctetString ThisPtr, ASN_Stream stream, U32 low, U32 high );

/** Get size in bits to store a encoded ASN_OctetString
***
*** @par Purpose:
*** Used by the generated code where an "OCTET STRING" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return The size in bits needed to encode this string.
***
***/
U32 ASN_OctetString_EncodedSize( ASN_OctetString ThisPtr, U32 low, U32 high );

/** Get string
***
*** @par Purpose:
*** Used to retreive the decoded data.
***
*** @param  ThisPtr String handle.
***
*** @param  data A byte pointer that will be set to the array of the string.
***
*** @return Length of string.
***
***/
U32 ASN_OctetString_Get( ASN_OctetString ThisPtr, ASN_BYTE * * data );

/** Get c-string
***
*** @par Purpose:
*** Used to retreive the data decoded for this string as a
*** null-terminated c-string. When decoding an ASN_OctetString an extra
*** byte is added containing a zero. You must still know by the protocol
*** that the specific field is a string as it otherwise could have
*** zero other than in the last byte.
***
*** @param  ThisPtr String handle.
***
*** @return Pointer to an null-terminated byte array.
***
***/
char * ASN_OctetString_GetText( ASN_OctetString ThisPtr );

#ifdef ENABLE_ASN_SESSION_SIZE
/** Get size in bytes needed to store an ASN_OctetString in a session
***
*** @par Purpose:
*** Used by the generated code where an "OCTET STRING" is declared in ASN.1.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return The size in bytes needed to decode this string into a session.
***
***/
U32 ASN_OctetString_SessionSize( U32 low, U32 high );
#endif // #ifdef ENABLE_ASN_SESSION_SIZE

/** Set a string
***
*** @par Purpose:
*** Used to copy a byte array into a string.
*** Useful if the session is supposed to contain all it's data.
***
*** @param  ThisPtr String handle.
***
*** @param  session Session handle.
***
*** @param  data Pointer with pre-allocated data to copy.
***
*** @param  length String length.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_OctetString_Set( ASN_OctetString ThisPtr, ASN_Session session, ASN_BYTE * data, U32 length );

/** Set as c-string
***
*** @par Purpose:
*** Used to copy a null-terminated string into a string.
*** Useful if the session is supposed to contain all it's data.
***
*** @param  ThisPtr String handle.
***
*** @param  session Session handle.
***
*** @param  text Pointer to a null-terminated string to be copied.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_OctetString_SetText( ASN_OctetString ThisPtr, ASN_Session session, char * text );

/** Associate a byte array with string
***
*** @par Purpose:
*** Used to be able to encode a byte array from it's original place.
***
*** @param  ThisPtr A string handle.
***
*** @param  data Pointer to pre-allocated data.
***
*** @param  length String length.
***
***/
void ASN_PrintableString_Associate( ASN_PrintableString ThisPtr, ASN_BYTE * data, U32 length );

/** Associate a null-terminated array with string
***
*** @par Purpose:
*** Used to be able to encode a null-terminated string from it's original place.
***
*** @param  ThisPtr String handle.
***
*** @param  text Pointer to a null-terminated string
***
***/
void ASN_PrintableString_AssociateText( ASN_PrintableString ThisPtr, char * text );

/** Get number of bits required to represent one character in a ASN_PrintableString
***
*** @par Purpose:
*** Used to retrieve the number of bits per character when encoding/decoding.
***
*** @param  low The minimum ASCII value in alphabet.
***
*** @param  high The maximum ASCII value in alphabet.
***
*** @return The size in bits needed to encode/decode one character.
***
***/
U32 ASN_PrintableString_BitsPerCharacter(U32 low, U32 high);

/** Create an ASN_PrintableString
***
*** @par Purpose:
*** Used by the generated code where an "PrintableString" is declared in ASN.1.
***
*** @param  session A string handle.
***
*** @return A pointer to the newly created string or NULL if 
***         there where not enough memory in the session.
***         The generated code will propagate the "right" error.
***
***/
ASN_PrintableString ASN_PrintableString_Create( ASN_Session session );

/** Decode an ASN_PrintableString
***
*** @par Purpose:
*** Used by the generated code where an "PrintableString" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  session A session handle.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @param  characterSet Allowed alphabet for string.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_PrintableString_Decode( ASN_PrintableString ThisPtr, ASN_Session session, ASN_Stream stream, U32 low, U32 high, const char * characterSet );

/** Encode an ASN_PrintableString
***
*** @par Purpose:
*** Used by the generated code where an "PrintableString" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @param  characterSet Allowed alphabet for string.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_PrintableString_Encode( ASN_PrintableString ThisPtr, ASN_Stream stream, U32 low, U32 high, const char * characterSet );

/** Get size in bits to store a encoded ASN_PrintableString
***
*** @par Purpose:
*** Used by the generated code where an "PrintableString" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @param  characterSet Allowed alphabet for string.
***
*** @return The size in bits needed to encode this string.
***
***/
U32 ASN_PrintableString_EncodedSize( ASN_PrintableString ThisPtr, U32 low, U32 high, const char * characterSet );

/** Finds character set position for character in string
***
*** @par Purpose:
*** Used to perform indexing during encoding.
***
*** @param  ThisPtr String handle.
***
*** @param  index Position in string.
***
*** @return Position for index in character set.
***
***/
ASN_BYTE ASN_PrintableString_FindIndexing(ASN_PrintableString ThisPtr, U32 index);

/** Get string
***
*** @par Purpose:
*** Used to retreive the decoded data.
***
*** @param  ThisPtr String handle.
***
*** @param  data A byte pointer that will be set to the array of the string.
***
*** @return Length of string.
***
***/
U32 ASN_PrintableString_Get( ASN_PrintableString ThisPtr, ASN_BYTE * * data );

/** Get c-string
***
*** @par Purpose:
*** Used to retreive the data decoded for this string as a
*** null-terminated c-string. When decoding an ASN_PrintableString an extra
*** byte is added containing a zero. You must still know by the protocol
*** that the specific field is a string as it otherwise could have
*** zero other than in the last byte.
***
*** @param  ThisPtr String handle.
***
*** @return Pointer to an null-terminated byte array.
***
***/
char * ASN_PrintableString_GetText( ASN_PrintableString ThisPtr );

/** Check if chars has a valid ASN_PrintableString alphabet
***
*** @par Purpose:
*** Used to check if the string that is to be encoded is within the allowed alphabet.
***
*** @param  ThisPtr String handle.
***
*** @param  chars Pointer with pre-allocated data.
***
*** @param  numberOfCharacters Length of string
***
*** @return True if all the characters in chars exists within the current character set.
***
***/
BOOL ASN_PrintableString_IsInCharacterSet(ASN_PrintableString ThisPtr, ASN_BYTE *chars, U32 numberOfCharacters);

/** Get character set range
***
*** @par Purpose:
*** Used to find the span of a characterSet.
***
*** @param  charSetMinValue Pointer to a character with lowest ASCII value.
***
*** @param  charSetMaxValue Pointer to a character with highest ASCII value.
***
*** @param  charSet Pointer to a null-terminated string.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
void ASN_PrintableString_MinMax(ASN_BYTE *charSetMinValue, ASN_BYTE *charSetMaxValue, ASN_BYTE * charSet);

#ifdef ENABLE_ASN_SESSION_SIZE
/** Get size in bytes needed to store an ASN_PrintableString in a session
***
*** @par Purpose:
*** Used by the generated code where an "PrintableString" is declared in ASN.1.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @param  characterSet Allowed alphabet for string.
***
*** @return The size in bytes needed to decode this string into a session.
***
***/
U32 ASN_PrintableString_SessionSize(U32 low, U32 high, const char * characterSet );
#endif /* #ifdef ENABLE_ASN_SESSION_SIZE */

/** Set a string
***
*** @par Purpose:
*** Used to copy a byte array into a string.
*** Useful if the session is supposed to contain all it's data.
***
*** @param  ThisPtr String handle.
***
*** @param  session Session handle.
***
*** @param  data Pointer with pre-allocated data to copy.
***
*** @param  length String length.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_PrintableString_Set( ASN_PrintableString ThisPtr, ASN_Session session, ASN_BYTE * data, U32 length );

/** Set the allowed character set ( alphabet )
***
*** @par Purpose:
*** Used to specify allowed alphabet for a printable string.
***
*** @param  ThisPtr String handle.
***
*** @param  charSet Pointer with pre-allocated data.
***
*** @param  charSetSize Number of characters in alphabet.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_PrintableString_SetCharacterSet(ASN_PrintableString ThisPtr, ASN_BYTE *charSet, U32 charSetSize);

/** Set as c-string
***
*** @par Purpose:
*** Used to copy a null-terminated string into a string.
*** Useful if the session is supposed to contain all it's data.
***
*** @param  ThisPtr String handle.
***
*** @param  session Session handle.
***
*** @param  text Pointer to a null-terminated string to be copied.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_PrintableString_SetText( ASN_PrintableString ThisPtr, ASN_Session session, char * data );

/** Associate a U16 array with ASN_RelativeObjectId
***
*** @par Purpose:
*** Used to be able to encode a U16 array from it's original place.
***
*** @param  ThisPtr A string handle.
***
*** @param  data Pointer to pre-allocated 16-bit data.
***
*** @param  The number of elements (arcs) of this 16-bit array..
***
***/
void ASN_RelativeObjectId_Associate( ASN_RelativeObjectId ThisPtr, U16 * data, U16 arcs );

/** Create an ASN_RelativeObjectId
***
*** @par Purpose:
*** Used by the generated code where an "RELATIVE-OID" is declared in ASN.1.
***
*** @param  session Session handle.
***
*** @return A pointer to the newly created string or NULL if 
***         there where not enough memory in the session.
***         The generated code will propagate the "right" error.
***
***/
ASN_RelativeObjectId ASN_RelativeObjectId_Create( ASN_Session session );

/** Decode an ASN_RelativeObjectId
***
*** @par Purpose:
*** Used by the generated code where an "RELATIVE-OID" is declared in ASN.1.
***
*** @param  ThisPtr ASN_RelativeObjectId handle.
***
*** @param  session Session handle.
***
*** @param  stream Stream handle.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_RelativeObjectId_Decode( ASN_RelativeObjectId ThisPtr, ASN_Session session, ASN_Stream stream );

/** Encode an ASN_RelativeObjectId
***
*** @par Purpose:
*** Used by the generated code where an "RELATIVE-OID" is declared in ASN.1.
***
*** @param  ThisPtr ASN_RelativeObjectId handle.
***
*** @param  stream Stream handle.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_RelativeObjectId_Encode( ASN_RelativeObjectId ThisPtr, ASN_Stream stream );

/** Get size in bits to store a encoded ASN_RelativeObjectId
***
*** @par Purpose:
*** Used by the generated code where an "RELATIVE-OID" is declared in ASN.1.
***
*** @param  ThisPtr ASN_RelativeObjectId handle.
***
*** @return The size in bits needed for encoding.
***
***/
U32 ASN_RelativeObjectId_EncodedSize( ASN_RelativeObjectId ThisPtr );

#ifdef ENABLE_ASN_SESSION_SIZE
/** Get size in bytes needed to store an ASN_RelativeObjectId in a session
***
*** @par Purpose:
*** Used by the generated code where an "RELATIVE-OID" is declared in ASN.1.
***
*** @return The size in bytes needed to decode this string into a session.
***
***/
U32 ASN_RelativeObjectId_SessionSize(  );
#endif /* #ifdef ENABLE_ASN_SESSION_SIZE */

/** Count number of bits required for representing low to high range of numbers.
***
*** @par Purpose:
*** Used by the generated functions.
***
*** @param  low The lower limit of the S32 value.
***
*** @param  high The higher limit of the S32 value.
***
*** @return The minimum number of bits needed to encode this range of numbers.
***
***/
U32 ASN_S32CountBits( S32 low, S32 high);

/** Checks the amount of free memory in a session.
***
*** @par Purpose:
*** Check the free memory in the session.
***
*** @param  session The session pointer.
***
*** @return The size in bytes of the remaining memory for session.
***
***/
U32 ASN_Session_BytesLeft( ASN_Session session );

/** Creates an ASN Session
***
*** @par Purpose:
*** Create a session with the memory that will contain the structs.
*** ASN_SESSION_SIZE is used internaly for the session. The rest
*** to ASN "objects".
***
*** @param  decoded_buffer A pointer to the memory used by the session.
***
*** @param  buf_size The size of the memory. Must be at least ASN_SESSION_SIZE.
***
*** @return ASN_Session The handle (pointer) for the session.
***
***/
ASN_Session ASN_Session_Create( ASN_BYTE * decoded_buffer, U32 buf_size );

/** Resets an ASN Session
***
*** @par Purpose:
*** To be able to reuse the same session.
***
*** @param  session The session pointer.
***
***/
void ASN_Session_Reset( ASN_Session session );

/** Decode a normally small unsigned
***
*** @par Purpose:
*** Decode a normally small unsigned variable from stream.
***
*** @param  stream The stream handle.
***
*** @param  value  U32 pointer for storing a normally small unsigned.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_SmallUnsigned_Decode( ASN_Stream stream, U32 *value);

/** Encode a normally small unsigned
***
*** @par Purpose:
*** Encode a normally small unsigned variable to stream.
***
*** @param  stream The stream handle.
***
*** @param  value  Normally small unsigned value.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_SmallUnsigned_Encode( ASN_Stream stream, U32 value);

/** Get size in bits to store a normally small unsigned in ASN_Stream
***
*** @par Purpose:
*** Used by the generated code ASN.1 specification.
***
*** @param  value Value to be encoded as a normally small unsigned.
***
*** @return The size in bits needed to encode a normally small unsigned on a stream.
***
***/
U32 ASN_SmallUnsigned_EncodedSize( U32 value );

/** Align internal bit/byte pointer on byte.
***
*** @par Purpose:
*** Needed when encoding several layers onto the same stream as many
*** layered protocol demands byte alignment between layers.
***
*** @param  stream A stream handle.
***
***/
void ASN_Stream_AlignOnByte( ASN_Stream stream );

/** Attach a memory buffer to a stream
***
*** @par Purpose:
*** If stream was created with size ASN_STREAM_SIZE then attaching a
*** buffer will be necessary in order to decode/encode with that stream. 
***
*** @param  stream A stream handle.
***
*** @param  buffer ASN_BYTE pointer to pre-allocated memory.
***
*** @param  size The size in bytes of the pre-allocated buffer.
***
***/
void ASN_Stream_AttachBuffer( ASN_Stream stream, ASN_BYTE * buffer, U32 size );

/** Get number of free bits in stream
***
*** @par Purpose:
*** Useful when manually encoding different protocol layers onto a stream.
***
*** @param  stream A stream handle.
***
*** @param  buffer ASN_BYTE pointer to pre-allocated memory.
***
*** @return  The space/memory left in the stream buffer to encode into, measured in bits.
***
***/
U32 ASN_Stream_BitsLeft( ASN_Stream stream );

/** Get number of used bytes in stream
***
*** @par Purpose:
*** Useful if you already have the pointer to the encoding/decoding buffer
*** but need the size to be able to send the message.
***
*** @param  stream A stream handle.
***
*** @return  Number of bytes consumed by encoding.
***
***/
U32 ASN_Stream_BytesUsed( ASN_Stream stream );

/** Create an ASN_Stream
***
*** @par Purpose:
*** Creating a stream that is used to decode or encode a message.
*** There are two ways of handling the memory in the stream.
*** 1. Create the stream with the size of both the internal need of
***    the stream and the encode buffer. I.e. ASN_STREAM_SIZE + 140.
*** 2. Create the stream with only memory for the internal need
***    for the stream and later attach a buffer.
***    I.e. the size of ASN_STREAM_SIZE only.
***
*** @param  buffer A pointer to the memory used by the stream.
***
*** @param  size The size in bytes of the buffer. Must be at least ASN_STREAM_SIZE.
***
*** @return A pointer to an ASN_Stream_t struct, i.e an ASN_Stream.
***
***/
ASN_Stream ASN_Stream_Create( ASN_BYTE * buffer, U32 size );

/** Get stream buffer
***
*** @par Purpose:
*** This function is useful if using a combined buffer for 
*** stream-internals and the encoding/decoding buffer itself,
*** or when passing only the stream handle to a function.
***
*** @param  stream The stream handle.
***
*** @param  size U32 pointer for storing buffer received size of buffer.
***
*** @return A pointer to the buffer.
***
***/
ASN_BYTE * ASN_Stream_GetBuffer( ASN_Stream stream, U32 * size );

/** Reset an ASN_Stream to its original state
***
*** @par Purpose:
*** To be able to encode or decode while re-using the stream.
***
*** @param stream The stream handle.
***
***/
void ASN_Stream_Reset( ASN_Stream stream );

/** Count number of bits required for representing low to high range of numbers.
***
*** @par Purpose:
*** Used by the generated functions.
***
*** @param  low The lower limit of the U32 value.
***
*** @param  high The higher limit of the U32 value.
***
*** @return The minimum number of bits needed to encode this range of numbers.
***
***/
U32 ASN_U32CountBits( U32 low, U32 high);

/** Associate a byte array with string
***
*** @par Purpose:
*** Used to be able to encode a byte array from it's original place.
***
*** @param  ThisPtr A string handle.
***
*** @param  data Pointer to pre-allocated data.
***
*** @param  length String length.
***
***/
void ASN_UTF8String_Associate( ASN_UTF8String ThisPtr, ASN_BYTE * data, U32 length );

/** Associate a null-terminated array with string
***
*** @par Purpose:
*** Used to be able to encode a null-terminated string from it's original place.
***
*** @param  ThisPtr String handle.
***
*** @param  text Pointer to a null-terminated string
***
***/
void ASN_UTF8String_AssociateText( ASN_UTF8String ThisPtr, char * text );

/** Create an ASN_UTF8String
***
*** @par Purpose:
*** Used by the generated code where an "UTF8String" is declared in ASN.1.
***
*** @param  session A string handle.
***
*** @return A pointer to the newly created string or NULL if 
***         there where not enough memory in the session.
***         The generated code will propagate the "right" error.
***
***/
ASN_UTF8String ASN_UTF8String_Create( ASN_Session session );

/** Decode an ASN_UTF8String
***
*** @par Purpose:
*** Used by the generated code where an "UTF8String" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  session A session handle.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_UTF8String_Decode( ASN_UTF8String ThisPtr, ASN_Session session, ASN_Stream stream, U32 low, U32 high );

/** Encode an ASN_UTF8String
***
*** @par Purpose:
*** Used by the generated code where an "UTF8String" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_UTF8String_Encode( ASN_UTF8String ThisPtr, ASN_Stream stream, U32 low, U32 high );

/** Get size in bits to store a encoded ASN_UTF8String
***
*** @par Purpose:
*** Used by the generated code where an "UTF8String" is declared in ASN.1.
***
*** @param  ThisPtr A string handle.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return The size in bits needed to encode this string.
***
***/
U32 ASN_UTF8String_EncodedSize( ASN_UTF8String ThisPtr, U32 low, U32 high );

/** Get string
***
*** @par Purpose:
*** Used to retreive the decoded data.
***
*** @param  ThisPtr String handle.
***
*** @param  data A byte pointer that will be set to the array of the string.
***
*** @return Length of string.
***
***/
U32 ASN_UTF8String_Get( ASN_UTF8String ThisPtr, ASN_BYTE * * data );

/** Get c-string
***
*** @par Purpose:
*** Used to retreive the data decoded for this string as a
*** null-terminated c-string. When decoding an ASN_UTF8String an extra
*** byte is added containing a zero. You must still know by the protocol
*** that the specific field is a string as it otherwise could have
*** zero other than in the last byte.
***
*** @param  ThisPtr String handle.
***
*** @return Pointer to an null-terminated byte array.
***
***/
char * ASN_UTF8String_GetText( ASN_UTF8String ThisPtr );

#ifdef ENABLE_ASN_SESSION_SIZE
/** Get size in bytes needed to store an ASN_UTF8String in a session
***
*** @par Purpose:
*** Used by the generated code where an "UTF8String" is declared in ASN.1.
***
*** @param  low The minimum length of this string.
***
*** @param  high The maximum length of this string.
***
*** @return The size in bytes needed to decode this string into a session.
***
***/
U32 ASN_UTF8String_SessionSize( U32 low, U32 high );
#endif /* #ifdef ENABLE_ASN_SESSION_SIZE */

/** Set a string
***
*** @par Purpose:
*** Used to copy a byte array into a string.
*** Useful if the session is supposed to contain all it's data.
***
*** @param  ThisPtr String handle.
***
*** @param  session Session handle.
***
*** @param  data Pointer with pre-allocated data to copy.
***
*** @param  length String length.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_UTF8String_Set( ASN_UTF8String ThisPtr, ASN_Session session, ASN_BYTE * data, U32 length );

/** Set as c-string
***
*** @par Purpose:
*** Used to copy a null-terminated string into a string.
*** Useful if the session is supposed to contain all it's data.
***
*** @param  ThisPtr String handle.
***
*** @param  session Session handle.
***
*** @param  text Pointer to a null-terminated string to be copied.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ASN_UTF8String_SetText( ASN_UTF8String ThisPtr, ASN_Session session, char * data );

/** Decode a BOOL value from stream
***
*** @par Purpose:
*** Encodes a BOOL value into a ASN_stream using 1 bit.
***
*** @param  value A BOOL pointer for storing a decoded bool.
***
*** @param  session The ASN_Session where storage will be allocated.
***
*** @param  stream A stream handle.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result BOOL_Decode( BOOL * value, ASN_Session session, ASN_Stream stream );

/** Encode a BOOL on stream
***
*** @par Purpose:
*** Encodes a BOOL value into a ASN_stream using 1 bit.
***
*** @param  value The BOOL number that should be encoded.
***
*** @param  stream A stream handle.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result BOOL_Encode( BOOL value, ASN_Stream stream );

/** Get size in bits to store a BOOL (always 1)
***
*** @par Purpose:
*** Used by the generated code where an "BOOLEAN" is declared in ASN.1.
***
*** @param  a BOOL value to be encoded.
***
*** @return The size in bits needed to encode a bool on a stream, i.e. 1 bit.
***
***/
U32 BOOL_EncodedSize(BOOL a);

/** Decode a ENUM
***
*** @par Purpose:
*** Used by the generated code where an "ENUMERATED" is declared in ASN.1.
***
*** @param  value U32 pointer to where decoded enum value shall be stored.
***
*** @param  session A session handle (UNUSED).
***
*** @param  stream A stream handle.
***
*** @param  low UNSUSED.
***
*** @param  maxEnumValue The maximum value of enum in root (extensions excluded).
***
*** @param  extensions ENUM type may have extension TRUE/FALSE.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ENUM_Decode( U32 *value, ASN_Session session, ASN_Stream stream,U32 low, U32 maxEnumValue, BOOL extensions);

/** Encode a ENUM
***
*** @par Purpose:
*** Used by the generated code where an "ENUMERATED" is declared in ASN.1.
***
*** @param  index Index of enum value.
***
*** @param  stream A stream handle.
***
*** @param  low UNSUSED.
***
*** @param  maxEnumValue Maximum value of enum in root (extensions excluded).
***
*** @param  extensions ENUM type may have extension TRUE/FALSE.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the error and line-number
***         in the generated code.
***
***/
ASN_Result ENUM_Encode( U32 value,  ASN_Stream stream,U32 low, U32 maxEnumValue, BOOL extensions);

/** Get size in bits to store an encoded enum
***
*** @par Purpose:
*** Calculate the number of bits required to store encoded enum value.
***
*** @param  value Value to be encoded as an enum.
***
*** @param  low Minimum value of enum.
***
*** @param  maxEnumValue Maximum value of enum in root.
***
*** @param  extensions Set to TRUE if this type may have extensions, otherwise FALSE.
***
*** @return The size in bits needed to encode this enum on a stream.
***
***/
U32 ENUM_EncodedSize(U32 value,U32 low, U32 maxEnumValue, BOOL extensions);

/** Get size in bytes needed to store an enum in a session
***
*** @param  low Minimum value of enum.
***
*** @param  maxEnumValue Maximum value of enum in root.
***
*** @param  extensions Set to TRUE if this type may have extensions, otherwise FALSE.
***
*** @return The number of bytes required to store enum value in a session.
***
*** @par    Purpose:
*** Calculate the number of bytes required for storing the decoded enum value.
***
***/
U32 ENUM_SessionSize(U32 low, U32 high, BOOL extensions);

/** Decode an signed value
***
*** @par Purpose:
*** Decodes a signed value into an ASN_Stream.
***
*** @param  value U32 pointer for storing decoded signed into.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum allowed size of this signed.
***
*** @param  high The maximum allowed size of this signed.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result S32_Decode( S32 * value, ASN_Session session, ASN_Stream stream, S32 low, S32 high );

/** Encode an signed value
***
*** @par Purpose:
*** Encodes a signed value into an ASN_Stream.
***
*** @param  value Value to be encoded as a signed.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum allowed size of this signed.
***
*** @param  high The maximum allowed size of this signed.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result S32_Encode( S32 value, ASN_Stream stream, S32 low, S32 high );

/** Get size in bits to store a size constrained signed
***
*** @par Purpose:
*** Used by the generated code where an "INTEGER(low..high)" is declared in ASN.1.
*** In the case where either one of or both low and high are negative.
*** The maximum range is limited to [low,high] = [INT_MIN,INT_MAX], ie the range of a 32-bit signed.
***
*** @param  value Value to be encoded as a signed.
***
*** @param  low The minimum size of this signed.
***
*** @param  high The maximum size of this signed.
***
*** @return The size in bits needed to encode a signed on a stream.
***
***/
U32 S32_EncodedSize(S32 a,S32 low, S32 high);

/** Get size in bytes needed to store a signed in a session
***
*** @par Purpose:
*** Used by the generated code where an "INTEGER(low..high)" is declared in ASN.1.
*** In the case where either one of or both low and high are negative.
*** The memory for these primitive types are allocated with the type itself
***
*** @param  low The minimum size of this signed.
***
*** @param  high The maximum size of this signed.
***
*** @return The size in bytes needed to decode a signed into a session.
***
***/
U32 S32_SessionSize(S32 a,S32 b);

/** Decode an unsigned value
***
*** @par Purpose:
*** Decodes an unsigned value into an ASN_Stream.
***
*** @param  value U32 pointer for storing decoded unsigned into.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum allowed size of this unsigned.
***
*** @param  high The maximum allowed size of this unsigned.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result U32_Decode( U32 * value, ASN_Session session, ASN_Stream stream, U32 low, U32 high );

/** Encode an unsigned value
***
*** @par Purpose:
*** Encodes an unsigned value into an ASN_Stream.
***
*** @param  value Value to be encoded as an unsigned.
***
*** @param  stream A stream handle.
***
*** @param  low The minimum allowed size of this unsigned.
***
*** @param  high The maximum allowed size of this unsigned.
***
*** @return ASN_RESULT_OK if OK or a ASN_Result with the the error and line-number
***         in the generated code.
***
***/
ASN_Result U32_Encode( U32 value, ASN_Stream stream, U32 low, U32 high );

/** Get size in bits to store a size constrained unsigned
***
*** @par Purpose:
*** Used by the generated code where an "INTEGER(low..high)" is declared in ASN.1.
*** In the case where both low and high are positive.
*** The maximum range is limited to [low,high] = [0,2^32-1], ie the range of a 32-bit unsigned.
***
*** @param  value Value to be encoded as an unsigned.
***
*** @param  low The minimum size of this unsigned.
***
*** @param  high The maximum size of this unsigned.
***
*** @return The size in bits needed to encode an unsigned on a stream.
***
***/
U32 U32_EncodedSize(U32 value,U32 low, U32 high);

/** Get size in bytes needed to store an unsigned in a session
***
*** @par Purpose:
*** Used by the generated code where an "INTEGER(low..high)" is declared in ASN.1.
*** In the case where both low and high are positive.
*** The memory for these primitive types are allocated with the type itself
***
*** @param  low The minimum size of this unsigned.
***
*** @param  high The maximum size of this unsigned.
***
*** @return The size in bytes needed to decode an unsigned into a session.
***
***/
U32 U32_SessionSize(U32 low,U32 high);

#ifdef ASN_ENABLE_TESTCODE

/* Executes all internal test code */
void ASN_TestCode( void );
/* If testcode is enabled, the printing must be enabled to. */
#ifndef ASN_PRINT
#define ASN_PRINT
#endif

#endif /* #ifdef ASN_ENABLE_TESTCODE */

#ifdef ASN_PRINT
/* Must be implemented from the "outside" if printing AND tests are enabled */
int ASN_PrintSetIndent( int i );
int ASN_Print( const char * format, ... );
int ASN_PrintIndented( const char * format, ... );
/* End of the "Must be implemented" */

#define U32_Print(a) ASN_PrintIndented("%lu" EOL, ( unsigned long)a)
#define S32_Print(a) ASN_PrintIndented("%ld" EOL, ( long)a)
#define BOOL_Print(a) ASN_PrintIndented("%s" EOL, (a)?"true":"false")
#define ASN_Null_Print(a) ASN_PrintIndented("NULL" EOL)
void ASN_ObjectId_Print( ASN_ObjectId ThisPtr );
void ASN_OctetString_Print( ASN_OctetString ThisPtr );
void ASN_PrintableString_Print( ASN_PrintableString ThisPtr );
void ASN_UTF8String_Print( ASN_UTF8String ThisPtr );
#endif /* #ifdef ASN_PRINT */

/// @}
#endif
