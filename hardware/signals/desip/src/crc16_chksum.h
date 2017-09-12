#ifndef CRC16_CHKSUM_H
#   define CRC16_CHKSUM_H
/*===========================================================================*/
/**
 * @file crc16_chksum.h
 *
 * Functions to checksum calculation
 *
 * %full_filespec: crc16_chksum.h~1:incl:bsd_mmm2#4 %
 * @version %version:1 %
 * @author  %derived_by: lzz760 %
 * @date    %date_modified: Mon Oct 31 12:49:34 2011 %
 *
 *------------------------------------------------------------------------------
 *
 * Copyright 2009 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Functions to CRC 16 checksum calculation
 *
 * @section ABBR ABBREVIATIONS:
 *   - CS - checksum
 *   - CRC - Cyclic Redundancy Check
 *
 * @section TRA TRACEABILITY INFO:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - <a href="http://kogespw1.delcoelect.com:8508/passthru/consumer?name=SW%20REF%20264.15D&doctype=K8DBSDoc">
 *       SW REF 264.15D "Delphi C Coding Standards" [12-Mar-2006]</a>
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None
 *
 * @defgroup checksum_grp Checksum calculation
 * @ingroup utilities_grp
 * @{
 */
/*==========================================================================*/

/*===========================================================================*\
 * Header Files
\*===========================================================================*/

/*===========================================================================*\
 * Exported Preprocessor #define Constants
\*===========================================================================*/


/*===========================================================================*\
 * Exported Preprocessor #define MACROS
\*===========================================================================*/

/*===========================================================================*\
 * Exported Type Declarations
\*===========================================================================*/

/*===========================================================================*\
 * Exported Const Object Declarations
\*===========================================================================*/

/*===========================================================================*\
 * Exported Function Prototypes
\*===========================================================================*/

/**
 * Calculates CRC 16 checksum of given data block
 *
 * @return Calculated CRC 16 checksum
 *
 * @param [in] start_addr   pointer of begin address of data to be checksumed
 * @param [in] len          number of bytes
 * @param [in] intial_value Intial Value
 *
 */
uint16_t CRC16_Calculate(const void *start_addr, uint16_t len, uint16_t intial_value);


/**
 * Verifies CRC 16 checksum
 *
 * @return bool true  - if the CRC is ok.
 *              false - if the CRC is nok.
 *
 * @param [in] start_addr   pointer of begin address of data to be checksumed
 * @param [in] len          number of bytes
 * @param [in] intial_value Intial Value
 * @param [in] chk_sum      16 bit CRC checksum
 *
 */
bool CRC16_Verify(const void *start_addr, uint16_t len, uint16_t intial_value, uint16_t chk_sum);

/*===========================================================================*\
 * Exported Inline Function Definitions and #define Function-Like Macros
\*===========================================================================*/

/*===========================================================================*/
/*!
 * @file crc16_chksum.h
 *
 * @section RH REVISION HISTORY (top to bottom: last revision to first revision)
 *
 * - 15-June-2009 Harish
 *  - Created initial file.
 */
/*===========================================================================*/
/** @} doxygen end group */
#endif /* CRC16_CHKSUM_H */

