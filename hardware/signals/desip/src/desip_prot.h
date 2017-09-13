/* Copyright 2005 Delphi Delco Electronics */

#pragma once

/*********************************************************/
/*       general defines                                 */
/*********************************************************/

#define SIP_RX_BUFSIZE  40   // Number of RX packets
#define SIP_TX_BUFSIZE  100  // Number of TX packets
#define SIP_MAXBYTE     9    // Number of bytes per packets is 2^SIP_MAXBYTE

/*REPLACE WITH TRUE CONSTANT LATER */
#define CLKS_PER_SEC    10000000

#define INT_MSB         (~(((unsigned int) ~0) >> 1))


/*********************************************************/
/*       flags                                           */
/*********************************************************/

#define FLG_TXFORCE     0x80
#define FLG_TXACK       0x40
#define FLG_TXFREE      0x20
#define FLG_RXFORCE     0x10
#define FLG_RXNAK       0x08
#define FLG_RXNAKMODE   0x04
#define FLG_GCSTATE     0x02
#define FLG_PCSTATE     0x01

#define FLD_CNTL        0x80
#define FLD_FORCE       0x40
#define FLD_NAK         0x20
#define FLD_CHKVER      0x10

#define CH_LO           0x1b
#define CH_HI           0x1c
#define CH_BIAS         (CH_HI-CH_LO+1)

#define SP_CH_LO        0x100
#define SP_CH_HI        0x101
#define SP_CH_ESC       0x100
#define SP_CH_SEP       0x101

#define NEW_CHK_SUM_VER 1
#define OLD_CHK_SUM_VER 0

/*********************************************************/
/*       states                                           */
/*********************************************************/


typedef enum
{
    RX_STATE_HDR = 0,
    RX_STATE_DATA
}
RX_STATES;

typedef enum
{
    TX_STATE_WAIT = 0,
    TX_STATE_PACK,
    TX_STATE_HDR1,
    TX_STATE_HDR2,
    TX_STATE_DATA,
    TX_STATE_CHK1,
    TX_STATE_CHK2,
    TX_STATE_SEP
}
TX_STATES;

typedef struct PKTNODE_tag
{
    int Len;
    char *pData;
    struct PKTNODE_tag *pNext;
}
PKTNODE;

typedef struct
{
    int     flags;
    int     InUse;
    int     (*pGetChr) (int);
    int     RxState;
    PKTNODE *RxPtr;
    PKTNODE RxMsg[SIP_RX_BUFSIZE];
    char    RxDat[SIP_RX_BUFSIZE * (1 << SIP_MAXBYTE)];
    PKTNODE *RxUsrPtr;
    unsigned char    RxHdr[4];
    int     RxLen;
    unsigned char    RxChk[2];
}
SIPCHAN_RX;

typedef struct
{
    int     flags;
    int     InUse;
    int     (*pPutChr) (int, int);
    int     PCCh;              /* storage for char that couldn't sent out before */
    int     TxState;
    PKTNODE *TxPtr;
    PKTNODE *TxOutPtr;
    PKTNODE *TxUsrPtr;
    PKTNODE TxMsg[SIP_TX_BUFSIZE];
    char    TxDat[SIP_TX_BUFSIZE * (1 << SIP_MAXBYTE)];
    int     TxInN;
    int     TxOutN;
    int     TxAckID;
    unsigned char    TxHdr[2];
    int     TxK;
    int     TxRxLen;
    unsigned char    TxChk[2];
    int     TxAlarm;
}
SIPCHAN_TX;

typedef struct
{
    int     Flags;
    int     RxID;
    int     RemoteHostMaxLen;
    int     RemoteHostMaxNP;
    int     chksum_version;
}
SIPCHAN_COMMON;

/*********************************************************/
/*       prototypes                                      */
/*********************************************************/

static int GetChr9 (int channelIdx);
static int PutChr9 (int c, int channelIdx);
static int PktListInit (PKTNODE **pp, PKTNODE *pMsg, char *pData, int Size, int Width);
static PKTNODE *PktListInsert (PKTNODE **ppList, PKTNODE *pMsg);
static PKTNODE *PktListExtract (PKTNODE **ppList);
static int PktListN (PKTNODE *pList);
static int TmrGet (void);
