/* Copyright 2005 Delphi Delco Electronics */

#include <stdio.h>
#include <string.h>             /* memcpy,strncpy */
#include <inttypes.h>

#include "desip_prot.h"
#include "desip_prot_if.h"
#include "crc16_chksum.h"
#include "vibdrvHWlink.h"
#undef LOG_TAG
#define LOG_TAG "DESIP"
#include <cutils/log.h>


#define DESIP_NUM_CHANNELS   (NUM_HW_LINKS)

#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
typedef struct Desip_Instru_Data_Tag
{
   uint32_t no_of_data_frame_xmit;
   uint32_t no_of_ctrl_frame_xmit;
   uint32_t no_of_data_frame_recv;
   uint32_t no_of_ctrl_frame_recv;
   uint32_t no_of_nack_frame_xmit;
   uint32_t no_of_frame_re_xmit;
   uint32_t no_of_checksum_err;
   uint32_t no_of_hw_err;
} Desip_Instru_Data_T;
#endif

#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
static Desip_Instru_Data_T desip_stat[DESIP_NUM_CHANNELS] = { 0 };
#endif
static SIPCHAN_TX  txChannel[DESIP_NUM_CHANNELS];
static SIPCHAN_RX  rxChannel[DESIP_NUM_CHANNELS];
static SIPCHAN_COMMON commonParas[DESIP_NUM_CHANNELS];  /* used by rx/tx */

static SIPCHAN_TX *tp[DESIP_NUM_CHANNELS];
static SIPCHAN_RX *rp[DESIP_NUM_CHANNELS];
static SIPCHAN_COMMON *cp[DESIP_NUM_CHANNELS];


static bool Is_UART_Rx_Error(int channelIdx);
static void TxStateChange(int channelIdx, TX_STATES newState);
static void RxStateChange(int channelIdx, RX_STATES newState);

int TmrGet (void)
{
    return (1);
}


/*
 * SIPOpen -- Opens a DESIP channel if one is available.  Does channel-specific
 *            initializations.  'pGetChr' and 'pPutChr' are callback functions
 *            used to receive/transmit single characters.  Returns a SIP_HANDLE
 *            which may be passed to other functions in this module.
 *            (e.g. SIPRecv())
 */
void SIPOpen (int (*pGetChr) (int), int (*pPutChr) (int, int), int channelIdx)
{
    tp[channelIdx] = &txChannel[channelIdx];
    rp[channelIdx] = &rxChannel[channelIdx];
    cp[channelIdx] = &commonParas[channelIdx];

    /* init common paras */
    cp[channelIdx]->RemoteHostMaxLen = 0;
    cp[channelIdx]->RemoteHostMaxNP = 0;
    cp[channelIdx]->Flags = FLG_RXFORCE;
    cp[channelIdx]->RxID = 0;
    cp[channelIdx]->chksum_version = NEW_CHK_SUM_VER; /* By default DESIP will try to transmit version 1 */

    /* init TX channel */
    tp[channelIdx]->TxRxLen = 1 << SIP_MAXBYTE;
    tp[channelIdx]->flags = FLG_TXFORCE;
    tp[channelIdx]->pPutChr = pPutChr;
    TxStateChange(channelIdx, TX_STATE_SEP);
    tp[channelIdx]->TxPtr = NULL;
    tp[channelIdx]->TxOutPtr = NULL;
    PktListInit (&tp[channelIdx]->TxUsrPtr, tp[channelIdx]->TxMsg, tp[channelIdx]->TxDat, SIP_TX_BUFSIZE, tp[channelIdx]->TxRxLen);
    tp[channelIdx]->TxInN = 0;
    tp[channelIdx]->TxOutN = 0;
    tp[channelIdx]->TxAckID = 0;
    tp[channelIdx]->TxK = 0;
    tp[channelIdx]->TxChk[0] = 0x00;
    tp[channelIdx]->TxChk[1] = 0x00;
    tp[channelIdx]->TxAlarm = TmrGet () + 1 * CLKS_PER_SEC;

    /* init RX channel */
    rp[channelIdx]->pGetChr = pGetChr;
    RxStateChange(channelIdx, RX_STATE_HDR);
    PktListInit (&rp[channelIdx]->RxPtr, rp[channelIdx]->RxMsg, rp[channelIdx]->RxDat, SIP_RX_BUFSIZE, tp[channelIdx]->TxRxLen);
    rp[channelIdx]->RxUsrPtr = NULL;
    rp[channelIdx]->RxLen = 4;
    rp[channelIdx]->RxChk[0] = 0x00;
    rp[channelIdx]->RxChk[1] = 0x00;
    rp[channelIdx]->flags = 0;

#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
   desip_stat[channelIdx].no_of_data_frame_xmit = 0;
   desip_stat[channelIdx].no_of_ctrl_frame_xmit = 0;
   desip_stat[channelIdx].no_of_data_frame_recv = 0;
   desip_stat[channelIdx].no_of_ctrl_frame_recv = 0;
   desip_stat[channelIdx].no_of_nack_frame_xmit = 0;
   desip_stat[channelIdx].no_of_frame_re_xmit   = 0;
   desip_stat[channelIdx].no_of_checksum_err    = 0;
   desip_stat[channelIdx].no_of_hw_err          = 0;
#endif
}

/*
 * SIPRecv -- Receives a DESIP data packet.  The returned packet contains data
 *            only.  Header, checksum, and separator information has been
 *            stripped.  Returns received packet length or -1 if no packets are
 *            available.
 */
int SIPRecv (char *pBuf, int channelIdx)
{
    int          Status = -1;
    PKTNODE      *pMsg;

    if ((pMsg = PktListExtract (&rp[channelIdx]->RxUsrPtr)) != NULL)
    {
        Status = pMsg->Len;
        memcpy (pBuf, pMsg->pData, Status);

        /* return node to input list */
        PktListInsert (&rp[channelIdx]->RxPtr, pMsg);
        /* trigger acknowledge */
        tp[channelIdx]->flags |= FLG_TXACK;   /* not clear !!! */
    }
    return Status;
}

/*
 * SIPXmit -- Transmit a DESIP data packet.  The packet should contain data
 *            only.  Header, checksum, and separator information is handled
 *            by the transmit state machine.  Returns the packet length or -1
 *            if there is no space available in the transmit packet buffer.
 */
int SIPXmit (char *pBuf, int Len, int channelIdx)
{
    int         Status = -1;
    PKTNODE     *pMsg;

    if ((pMsg = PktListExtract (&tp[channelIdx]->TxUsrPtr)) != NULL)
    {
        if (Len > tp[channelIdx]->TxRxLen)
        {
            ALOGW("SIPXmit: DESIP payload len %d is too long. Max len is %d",Len,tp[channelIdx]->TxRxLen);
            Len = tp[channelIdx]->TxRxLen;
        }
        Status = pMsg->Len = Len;
        memcpy (pMsg->pData, pBuf, Status);

        PktListInsert (&tp[channelIdx]->TxPtr, pMsg);
        tp[channelIdx]->TxInN++;
    }
    else
    {
        ALOGW("SIPXmit: Msg not transmitted due to no free buffers");
    }
    return Status;
}

/*
 * GetChr9 -- Implements receive escape sequences as follows:
 *
 *            Input                      Output
 *            --------------------------+--------------------------
 *            0x00..0x1a and 0x1d..0xff | 0x00..0x1a and 0x1d..0xff
 *            0x1b,0x1d                 | 0x1b
 *            0x1b,0x1e                 | 0x1c
 *            0x1c                      | 0x101
 *
 *            Returns the received character or -1 if none are available.
 */
static int GetChr9 (int channelIdx)
{
    int Done = 0;
    int c;

    while (!Done)
    {
        Done = 1;
        if ((c = (*rp[channelIdx]->pGetChr) (channelIdx)) != -1)
        {
            if (!(rp[channelIdx]->flags & FLG_GCSTATE))
            {
                if ((CH_LO <= c) && (c <= CH_HI))
                {
                    c += SP_CH_LO - CH_LO;
                    if (c == SP_CH_ESC)
                    {
                        rp[channelIdx]->flags |= FLG_GCSTATE;
                        Done = 0;
                    }
                }
            }
            else
            {
                rp[channelIdx]->flags &= ~FLG_GCSTATE;
                c -= CH_BIAS;
                if ((c < CH_LO) || (c > CH_HI))
                {
                    Done = 0;
                }
            }
        }
    }
    return c;
}

/*
 * PutChr9 -- Implements transmit escape sequences as follows:
 *
 *            Input                      Output
 *            --------------------------+--------------------------
 *            0x00..0x1a and 0x1d..0xff | 0x00..0x1a and 0x1d..0xff
 *            0x1b                      | 0x1b,0x1d
 *            0x1c                      | 0x1b,0x1e
 *            0x101                     | 0x1c
 *
 *            Returns the transmitted character or -1 if the character is
 *            rejected.
 */
static int PutChr9 (int c, int channelIdx)
{
    int Done = 0;
    int Status;

    Status = c;
    while (!Done)
    {
        Done = 1;
        if (!(tp[channelIdx]->flags & FLG_PCSTATE))
        {
            if (c != -1)
            {
                if ((c < CH_LO) || (c > CH_HI))
                {
                    if ((SP_CH_LO <= c) && (c <= SP_CH_HI))
                    {
                        c -= SP_CH_LO - CH_LO;
                    }
                    if ((*tp[channelIdx]->pPutChr) (c, channelIdx) == -1)
                    {
                        Status = -1;
                    }
                }
                else
                {
                    if ((*tp[channelIdx]->pPutChr) (SP_CH_ESC - (SP_CH_LO - CH_LO), channelIdx) != -1)
                    {
                        c += CH_BIAS;
                        if ((*tp[channelIdx]->pPutChr) (c, channelIdx) == -1)
                        {
                           tp[channelIdx]->PCCh = c;
                           tp[channelIdx]->flags |= FLG_PCSTATE;
                        }
                    }
                    else
                    {
                        Status = -1;
                    }
                }
            }
        }
        else
        {
            if ((*tp[channelIdx]->pPutChr) (tp[channelIdx]->PCCh, channelIdx) != -1)
            {
                tp[channelIdx]->flags &= ~FLG_PCSTATE;
                Done = 0;
            }
            else
            {
                Status = -1;
            }
        }
    }
    return Status;
}

/*
 * RxFSM -- DESIP receive state machine.  Runs until it cannot proceed which is
 *          when GetChr9() returns -1.
 *
 *          Note: removed variables
 *          unsigned char errorBuf[150]
 *          int errorLen, i, j;
 *
 */
void SIP_Rx_FSM (int channelIdx)
{
    int Done;
    int c;
    int Hdr0;
    int Hdr1;
    unsigned int chksum;
    int check_sum_valid;
    int count;
    static int chksum_version[DESIP_NUM_CHANNELS] = {2, 2, 2};
    bool is_rx_err;

    while ((c = GetChr9 (channelIdx)) != -1)
    {
        switch (rp[channelIdx]->RxState)
        {
            case RX_STATE_HDR:
            {
                if ((0x0000 <= c) && (c <= 0x00ff))
                {
                    rp[channelIdx]->RxHdr[4 - rp[channelIdx]->RxLen] = (unsigned char) c;
                    /* Is it 3 Byte Header */
                    if ((!(rp[channelIdx]->RxHdr[0] & FLD_CHKVER)) && (2 == rp[channelIdx]->RxLen))
                    {
                         /* Skip the reception of the CHKSUM 2nd Byte */
                         rp[channelIdx]->RxLen=1;
                    }
                    if (--rp[channelIdx]->RxLen <= 0)
                    {
                        RxStateChange(channelIdx, RX_STATE_DATA);
                    }
                }
                else
                {
                    if (c == SP_CH_SEP)
                    {
                        SIP_Tx_FSM (TX_FSM_EVENT_TXACK, 0x00, channelIdx);
                        cp[channelIdx]->Flags |= FLG_RXNAK;
                        rp[channelIdx]->RxLen = 4;
                        rp[channelIdx]->RxChk[0] = 0x00;
                        rp[channelIdx]->RxChk[1] = 0x00;
                    }
                }
            }
            break;

            case RX_STATE_DATA:
            {
                if ((0x0000 <= c) && (c <= 0x00ff))
                {
                    /* receive buffer available? */
                    if (rp[channelIdx]->RxPtr && (rp[channelIdx]->RxLen < tp[channelIdx]->TxRxLen))
                    {
                        /* 8 -bit Checksum Logic */
                        if(!(rp[channelIdx]->RxHdr[0] & FLD_CHKVER))
                        {
                            /* save last received data byte */
                            rp[channelIdx]->RxPtr->pData[rp[channelIdx]->RxLen] = rp[channelIdx]->RxHdr[2];
                            /* current data is stored temporarily in rx_HDR[2] */
                            rp[channelIdx]->RxHdr[2] = (unsigned char)c;       /* last data to checksum */
                        }
                        else /* 16 bit logic */
                        {
                            /* last to last received data byte */
                            rp[channelIdx]->RxPtr->pData[rp[channelIdx]->RxLen] = rp[channelIdx]->RxHdr[2];
                            /* last received data byte */
                            rp[channelIdx]->RxHdr[2] = rp[channelIdx]->RxHdr[3];
                            /* current data is stored temporarily in rx_HDR[3] */
                            rp[channelIdx]->RxHdr[3] = (unsigned char)c;       /* last data to checksum */
                        }
                    }
                    rp[channelIdx]->RxLen++;
                }
                else
                {
                    if (c == SP_CH_SEP)
                    {
                        /* if packet is valid, decode it, else force status */
                        Done = 1;
                        Hdr0 = (int) rp[channelIdx]->RxHdr[0] & 0xff;
                        Hdr1 = (int) rp[channelIdx]->RxHdr[1] & 0xff;
                        /* Calculate the checksum here */
                        if (rp[channelIdx]->RxHdr[0] & FLD_CHKVER)
                        {
                            /* Calculate the 16 bit checksum */

                            chksum = 0;
                            chksum = CRC16_Calculate (&rp[channelIdx]->RxHdr[0], 1, chksum);
                            chksum = CRC16_Calculate (&rp[channelIdx]->RxHdr[1], 1, chksum);
                            if (!(Hdr0 & FLD_CNTL)) /* Only if it is Data Frame */
                            {
                                chksum = CRC16_Calculate (&(rp[channelIdx]->RxPtr->pData[0]),rp[channelIdx]->RxLen,chksum);
                            }
                            rp[channelIdx]->RxChk[0] = (unsigned char)(chksum & 0xff);
                            rp[channelIdx]->RxChk[1] = (unsigned char)((chksum >> 8) & 0xff);
                        }
                        else
                        {
                            /* Calculate the old 8 bit checksum */
                            rp[channelIdx]->RxChk[0] = (rp[channelIdx]->RxHdr[0] + rp[channelIdx]->RxHdr[1]);
                            if (!(rp[channelIdx]->RxHdr[0] & FLD_CNTL)) /* Only if it is Data Frame */
                            {
                                 /* Total Data only bytes will be p->rx_Len - 2 */
                                 for (count = 1; count <= rp[channelIdx]->RxLen; count++)
                                 {
                                      rp[channelIdx]->RxChk[0] += (rp[channelIdx]->RxPtr->pData[count - 1] ^ count);
                                 }
                            }
                        }
                        check_sum_valid = 0;

                        /* Check if the Check sum is valid */
                        if (rp[channelIdx]->RxHdr[0] & FLD_CHKVER)
                        {
                            /* Decode the 16 Bit Check Sum */
                            if ((rp[channelIdx]->RxChk[0] == rp[channelIdx]->RxHdr[2])&&
                                (rp[channelIdx]->RxChk[1] == rp[channelIdx]->RxHdr[3]))
                            {
                                check_sum_valid = 1;
                            }
                        }
                        else
                        {
                            /* Decode the 8 Bit Check Sum */
                            if ((rp[channelIdx]->RxChk[0] + rp[channelIdx]->RxHdr[2])==0xff)
                            {
                               check_sum_valid = 1;
                            }
                        }
                        is_rx_err = Is_UART_Rx_Error(channelIdx);
                        if ((1 == check_sum_valid) && (is_rx_err == false))
                        {
                            /* Decode the frame Version here .... */
                            if(!(rp[channelIdx]->RxHdr[0] & FLD_CHKVER))
                            {
                               cp[channelIdx]->chksum_version = OLD_CHK_SUM_VER;
                               if(chksum_version[channelIdx] != cp[channelIdx]->chksum_version)
                               {
                                  chksum_version[channelIdx] = cp[channelIdx]->chksum_version;
                                  ALOGI("DESIP checksum version: %d 8 bit CRC checksum, channel index %d",chksum_version[channelIdx],channelIdx);
                               }
                            }
                            else
                            {
                               cp[channelIdx]->chksum_version = NEW_CHK_SUM_VER;
                               if(chksum_version[channelIdx] != cp[channelIdx]->chksum_version)
                               {
                                  chksum_version[channelIdx] = cp[channelIdx]->chksum_version;
                                  ALOGI("DESIP checksum version: %d 16 bit CRC checksum, channel index %d", chksum_version[channelIdx],channelIdx);
                               }
                            }
#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
                            if (FLD_CNTL == (Hdr0 & FLD_CNTL))
                            {
                                desip_stat[channelIdx].no_of_ctrl_frame_recv++;
                            }
                            else
                            {
                                desip_stat[channelIdx].no_of_data_frame_recv++;
                            }
#endif
                            /* check for sync packet */
                            if ((Hdr0 & (FLD_CNTL | FLD_FORCE | FLD_NAK | 0x0f))
                                 == (FLD_CNTL | FLD_FORCE | FLD_NAK))
                            {
                                /* trigger TX state machine to sync */
                                cp[channelIdx]->Flags |= FLG_RXFORCE | FLG_RXNAK;
                                cp[channelIdx]->RemoteHostMaxNP = 0;
                                SIP_Tx_FSM (TX_FSM_EVENT_SYNC, Hdr1, channelIdx);
                            }
                            /* check for sync mode */
                            else
                            {
                                if (!cp[channelIdx]->RemoteHostMaxLen) /* no rx data len of remote host known */
                                {
                                    cp[channelIdx]->Flags |= FLG_RXFORCE;
                                    if (Hdr0 & FLD_CNTL)
                                    {
                                        if (cp[channelIdx]->RemoteHostMaxNP > 0)
                                        {
                                            cp[channelIdx]->RemoteHostMaxNP--;
                                        }
                                        else
                                        {
                                            SIP_Tx_FSM (TX_FSM_EVENT_TXFORCE, (Hdr1 & 0xf), channelIdx);
                                            Done = 0;
                                        }
                                    }
                                }
                                else
                                {
                                    Done = 0;
                                }
                            }
                        }
                        else
                        {
                            if (check_sum_valid == 0)
                            {
#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
                                desip_stat[channelIdx].no_of_checksum_err++;
#endif
                                ALOGW("HiSIP: Checksum Incorrect Sending NACK to VIP");
                            }
                            SIP_Tx_FSM (TX_FSM_EVENT_TXACK, 0x00, channelIdx);
                            cp[channelIdx]->Flags |= FLG_RXNAK;
                        }

                        if (!Done)
                        {
                            cp[channelIdx]->RemoteHostMaxNP = (Hdr1 >> 4) & 0xf;
                            Hdr1 &= 0xf;

                            /* free (re)transmit buffer space used by ACK'd packets */
                            SIP_Tx_FSM (TX_FSM_EVENT_FREEBUF, Hdr1, channelIdx);
                            if (Hdr0 & FLD_FORCE)
                            {
                                SIP_Tx_FSM (TX_FSM_EVENT_SETFORCE, 0x00, channelIdx);
                            }
                            if (Hdr0 & FLD_NAK)
                            {
                                SIP_Tx_FSM (TX_FSM_EVENT_NAK, 0x00, channelIdx);
#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
                                desip_stat[channelIdx].no_of_nack_frame_xmit++;
#endif
                            }
                            if (Hdr0 & FLD_CNTL)
                            {
                                cp[channelIdx]->RemoteHostMaxLen = 1 << (Hdr0 & 0xf);
                            }
                            else
                            {
                                c = ( (Hdr0 & 0xf) - cp[channelIdx]->RxID - 1) & 0xf;
#if defined(XIS_SIPPROT_INSTRUMENTED_FOR_STAT)
                                if (c)
                                {
                                    /* rx_hdr_byte0 &0xf - Recevied Tx ID */
                                    /* p->rx_ID          - previous Tx ID */

                                    /* Acc.. to protocol
                                    * received Tx Id should be 1 more than previous
                                    *
                                    */
                                    desip_stat[channelIdx].no_of_frame_re_xmit += (c + 1);
                                }
#endif
                                if (rp[channelIdx]->RxPtr && !c)
                                {
                                    /* data packet is accepted */
                                    cp[channelIdx]->RxID = (cp[channelIdx]->RxID + 1) & 0xf;
                                    cp[channelIdx]->Flags &= ~FLG_RXNAKMODE;
                                    if (rp[channelIdx]->RxLen > tp[channelIdx]->TxRxLen)
                                    {
                                        rp[channelIdx]->RxLen = tp[channelIdx]->TxRxLen;
                                    }
                                    rp[channelIdx]->RxPtr->Len = rp[channelIdx]->RxLen;
                                    PktListInsert (&rp[channelIdx]->RxUsrPtr,
                                                   PktListExtract (&rp[channelIdx]->RxPtr));
                                }
                                else
                                {
                                    if (!(cp[channelIdx]->Flags & FLG_RXNAKMODE) && (c < 15))
                                    {
                                        /* improper packet number -- do NAK */
                                        cp[channelIdx]->Flags |= FLG_RXNAKMODE | FLG_RXNAK;
                                        SIP_Tx_FSM (TX_FSM_EVENT_RXNACK, 0x00, channelIdx);
                                    }
                                    else
                                    {
                                        SIP_Tx_FSM (TX_FSM_EVENT_TXACK, 0x00, channelIdx);
                                    }
                                }
                            }
                        }  /* if !Done */
                        rp[channelIdx]->RxLen = 4;
                        rp[channelIdx]->RxChk[0] = 0x00;
                        rp[channelIdx]->RxChk[1] = 0x00;
                        RxStateChange(channelIdx, RX_STATE_HDR);
                    } /* if (c == SP_CH_SEP) */
                }
            }
            break;

            default:
            {
                RxStateChange(channelIdx, RX_STATE_HDR);
                break;
            }
        }
    }
}

/*
 * TxFSM -- DESIP transmit state machine.  Runs until it cannot proceed which is
 *          when either PutChr9() returns -1 or there is nothing to do.
 */
void SIP_Tx_FSM (TX_FSM_EVENT ev, unsigned char para, int channelIdx)
{
    unsigned int chksum;
    int Done = 0;
    int c    = 0;
    int count;
    int rxCnt;

    switch(ev)
    {
        case TX_FSM_EVENT_SYNC:
        {
            tp[channelIdx]->TxInN += tp[channelIdx]->TxOutN;
            tp[channelIdx]->TxOutN = 0;
            tp[channelIdx]->TxAckID = para & 0xf;
            tp[channelIdx]->flags |= FLG_TXFORCE;
            break;
        }
        case TX_FSM_EVENT_TXFORCE:
        {
            tp[channelIdx]->TxAckID = para;
            tp[channelIdx]->flags |= FLG_TXFORCE;
            break;
        }
        case TX_FSM_EVENT_RXNACK:
        {
            tp[channelIdx]->flags |= FLG_TXACK;
            break;
        }
        case TX_FSM_EVENT_NAK:
        {
            tp[channelIdx]->TxInN += tp[channelIdx]->TxOutN;
            tp[channelIdx]->TxOutN = 0;
            break;
        }
        case TX_FSM_EVENT_SETFORCE:
        {
            tp[channelIdx]->flags |= FLG_TXFORCE;
            break;
        }
        case TX_FSM_EVENT_TXACK:
        {
            tp[channelIdx]->flags |= FLG_TXACK;
            break;
        }
        case TX_FSM_EVENT_FREEBUF:
        {
            while (tp[channelIdx]->TxAckID != para)
            {
                if (!tp[channelIdx]->TxPtr)
                {
                    tp[channelIdx]->TxInN = tp[channelIdx]->TxOutN = 0;
                }
                else
                {
                    if (tp[channelIdx]->TxPtr == tp[channelIdx]->TxOutPtr)
                    {
                        PktListExtract (&tp[channelIdx]->TxPtr);
                        tp[channelIdx]->flags |= FLG_TXFREE;
                    }
                    else
                    {
                        PktListInsert (&tp[channelIdx]->TxUsrPtr,
                                       PktListExtract (&tp[channelIdx]->TxPtr));
                    }
                }
                if (tp[channelIdx]->TxOutN > 0)
                {
                    tp[channelIdx]->TxOutN--;
                }
                else
                {
                    tp[channelIdx]->TxInN = PktListN (tp[channelIdx]->TxPtr);
                }
                tp[channelIdx]->TxAckID++;
                tp[channelIdx]->TxAckID &= 0xf;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    if (ev != TX_FSM_EVENT_NORMAL_OP)
    {
        return;
    }

    while (!Done)
    {
        switch (tp[channelIdx]->TxState)
        {
            case TX_STATE_WAIT:
            {
                if ((tp[channelIdx]->flags & FLG_TXFORCE))
                {
                    /* xmit a control-only packet */
                    if (cp[channelIdx]->RemoteHostMaxLen || !cp[channelIdx]->RemoteHostMaxNP)
                    {
                        c = FLD_CNTL | SIP_MAXBYTE;
                    }
                    else
                    {
                        c = FLD_CNTL | FLD_FORCE | FLD_NAK;
                    }
                    tp[channelIdx]->flags &= ~FLG_TXFORCE;
                    TxStateChange(channelIdx, TX_STATE_PACK);
                }
                else
                {
                    if ((tp[channelIdx]->TxInN > 0) && (cp[channelIdx]->RemoteHostMaxNP > tp[channelIdx]->TxOutN) && cp[channelIdx]->RemoteHostMaxLen)
                    {
                        tp[channelIdx]->TxOutPtr = tp[channelIdx]->TxPtr;
                        if (tp[channelIdx]->TxOutPtr == NULL)
                        {
                            Done = 1;
                            break;
                        }
                        for (c = 0; c < tp[channelIdx]->TxOutN; c++)
                        {
                            tp[channelIdx]->TxOutPtr = tp[channelIdx]->TxOutPtr->pNext;
                            if (tp[channelIdx]->TxOutPtr == NULL)
                            {
                                Done = 1;
                                break;
                            }
                        }
                        if (tp[channelIdx]->TxOutPtr == NULL)
                        {
                            Done = 1;
                            break;
                        }
                        tp[channelIdx]->TxInN--;
                        tp[channelIdx]->TxOutN++;
                        c = tp[channelIdx]->TxAckID + tp[channelIdx]->TxOutN;
                        c &= 0xf;
                        tp[channelIdx]->TxAlarm = TmrGet () + 1 * CLKS_PER_SEC;
                        TxStateChange(channelIdx, TX_STATE_PACK);
                    }
                    else
                    {
                        if ((tp[channelIdx]->flags & FLG_TXACK))
                        {
                            /* nothing else to do, so do any pending ACK */
                            tp[channelIdx]->flags |= FLG_TXFORCE;
                        }
                        else
                        {
                            if ((tp[channelIdx]->TxAlarm - TmrGet ()) & INT_MSB)
                            {
                                tp[channelIdx]->TxAlarm = TmrGet () + 1 * CLKS_PER_SEC;
                                if (tp[channelIdx]->TxInN + tp[channelIdx]->TxOutN > 0)
                                {
                                    /* packets should be ACK'd by now, retransmit unACK'd packets */
                                    tp[channelIdx]->TxInN += tp[channelIdx]->TxOutN;
                                    tp[channelIdx]->TxOutN = 0;
                                    cp[channelIdx]->Flags |= FLG_RXFORCE;
                                    tp[channelIdx]->flags |= FLG_TXACK;
                                }
                            }
                            else
                            {
                                Done = 1;
                            }
                        }
                    }
                }
            }
            break;

            case TX_STATE_PACK:
            {
                if ((cp[channelIdx]->Flags & FLG_RXFORCE))
                {
                    c |= FLD_FORCE;
                }

                if ((cp[channelIdx]->Flags & FLG_RXNAK))
                {
                    c |= FLD_NAK;
                }

                tp[channelIdx]->flags &= ~(FLG_TXACK);
                cp[channelIdx]->Flags &= ~(FLG_RXFORCE | FLG_RXNAK);

                tp[channelIdx]->TxHdr[0] = (unsigned char) c;

                /* Encode - RxNP - Number of packets this node is able to receive */
                rxCnt = PktListN(rp[channelIdx]->RxPtr);

                /* Ensure number of free buffers is not larger than 15, it needs to fit into a nibble */
                if (rxCnt > 15)
                {
                   rxCnt = 15;
                }

                /* Encode ID of the last received packet - Acknowledge */
                c = (rxCnt << 4) | cp[channelIdx]->RxID;
                tp[channelIdx]->TxHdr[1] = (unsigned char) c;

                /* Fill in the version here */
                if (NEW_CHK_SUM_VER == cp[channelIdx]->chksum_version)
                {
                    /* Fill in the version here */
                    tp[channelIdx]->TxHdr[0] |= FLD_CHKVER;
                }
                else
                {
                    /* Fill in the version here */
                    tp[channelIdx]->TxHdr[0] &= ~FLD_CHKVER;
                }
                tp[channelIdx]->TxChk[0] = 0x00;            /*Intialize the Checksum to 0*/
                tp[channelIdx]->TxChk[1] = 0x00;
                tp[channelIdx]->TxK = 0;
                TxStateChange(channelIdx, TX_STATE_HDR1);
            }
            break;

            case TX_STATE_HDR1:
            {
                if (PutChr9 ((int) tp[channelIdx]->TxHdr[0] & 0xff, channelIdx) != -1)
                {
                    TxStateChange(channelIdx, TX_STATE_HDR2);
                }
                else
                {
                    Done = 1;
                }
            }
            break;

            case TX_STATE_HDR2:
            {
                if (PutChr9 ((int) tp[channelIdx]->TxHdr[1] & 0xff, channelIdx) != -1)
                {
                    if ((int)tp[channelIdx]->TxHdr[0] & FLD_CNTL)
                    {
                        TxStateChange(channelIdx, TX_STATE_CHK1);
#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
                        desip_stat[channelIdx].no_of_ctrl_frame_xmit++;
#endif
                    }
                    else
                    {
                        TxStateChange(channelIdx, TX_STATE_DATA);
#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
                        desip_stat[channelIdx].no_of_data_frame_xmit++;
#endif
                    }
                }
                else
                {
                    Done = 1;
                }
            }
            break;

            case TX_STATE_DATA:
            {
                Done = 1;
                while (PutChr9 (c = (int) tp[channelIdx]->TxOutPtr->pData[tp[channelIdx]->TxK] & 0xff, channelIdx) != -1)
                {
                    /* xmit all bytes of packet except the checksum */
                    ++tp[channelIdx]->TxK;
                    if (tp[channelIdx]->TxK >= tp[channelIdx]->TxOutPtr->Len)
                    {
                        Done = 0;
                        TxStateChange(channelIdx, TX_STATE_CHK1);
                        break;
                    }
                }
            }
            break;

            case TX_STATE_CHK1:
            {
                /* Calculate the checksum here */
                if (cp[channelIdx]->chksum_version == NEW_CHK_SUM_VER)
                {
                    /* Calculate the 16 bit checksum */
                    chksum = 0;
                    chksum = CRC16_Calculate (&tp[channelIdx]->TxHdr[0], 1, chksum);
                    chksum = CRC16_Calculate (&tp[channelIdx]->TxHdr[1], 1, chksum);
                    if (!(tp[channelIdx]->TxHdr[0] & FLD_CNTL)) /* Only if it is Data Frame */
                    {
                          chksum = CRC16_Calculate(&(tp[channelIdx]->TxOutPtr->pData[0]),
                          tp[channelIdx]->TxOutPtr->Len, chksum);
                    }
                    tp[channelIdx]->TxChk[0] = (unsigned char)(chksum & 0xff);
                    tp[channelIdx]->TxChk[1] = (unsigned char)((chksum>>8) & 0xff);
                }
                else
                {
                    /* Calculate the old 8 bit checksum */
                    tp[channelIdx]->TxChk[0]=(tp[channelIdx]->TxHdr[0] + tp[channelIdx]->TxHdr[1]);
                    if (!(tp[channelIdx]->TxHdr[0] & FLD_CNTL)) /* Only if it is Data Frame */
                    {
                        for (count=1; count<=tp[channelIdx]->TxOutPtr->Len; count++)
                        {
                            tp[channelIdx]->TxChk[0]+=(tp[channelIdx]->TxOutPtr->pData[count-1] ^ count);
                        }
                    }
                     tp[channelIdx]->TxChk[0]=~tp[channelIdx]->TxChk[0];
                }
                if (PutChr9 (tp[channelIdx]->TxChk[0] & 0xff, channelIdx) != -1)
                {
                    if (cp[channelIdx]->chksum_version == NEW_CHK_SUM_VER)
                    {
                        TxStateChange(channelIdx, TX_STATE_CHK2); /* set transmit state to transmit separator*/
                    }
                    else
                    {
                        TxStateChange(channelIdx, TX_STATE_SEP); /* set transmit state to transmit separator*/
                    }
                }
                else
                {
                    Done = 1;
                }
            }
            break;
            case TX_STATE_CHK2:
            {
                 if (PutChr9(tp[channelIdx]->TxChk[1] & 0xff, channelIdx) != -1)
                 {
                     TxStateChange(channelIdx, TX_STATE_SEP); /* set transmit state to transmit separator*/
                 }
                 else
                 {
                       Done = 1; /* EXIT transmit state */
                 }
            }
            break;
            case TX_STATE_SEP:
            {
                if (PutChr9 (SP_CH_SEP, channelIdx) != -1)
                {
                    if ((tp[channelIdx]->flags & FLG_TXFREE))
                    {
                        if (tp[channelIdx]->TxOutPtr)
                        {
                            PktListInsert (&tp[channelIdx]->TxUsrPtr, tp[channelIdx]->TxOutPtr);
                        }
                        tp[channelIdx]->flags &= ~FLG_TXFREE;
                    }
                    tp[channelIdx]->TxOutPtr = NULL;
                    TxStateChange(channelIdx, TX_STATE_WAIT);
                }
                else
                {
                    Done = 1;
                }
            }
            break;

            default:
            {
                TxStateChange(channelIdx, TX_STATE_WAIT);
                break;
            }
        }
    }
}

/*
 * PktListInit -- Initializes a linked list of packets
 */
static int PktListInit (PKTNODE  **ppList,
                        PKTNODE  *pMsg,
                        char     *pData,
                        int      Size,
                        int      Width)
{
    int k;

    *ppList = NULL;
    for (++Size; --Size; pMsg++)
    {
        pMsg->Len = 0;
        pMsg->pData = pData;
        for (k = 0; k < Width; k++)
        {
            *pData++ = '\0';
        }
        pMsg->pNext = *ppList;
        *ppList = pMsg;
    }
    return 0;
}

/*
 * PktListInsert -- Appends a packet to a linked list
 */
static PKTNODE  *PktListInsert (PKTNODE   **ppList,
                                PKTNODE   *pMsg)
{
    PKTNODE *p;
    PKTNODE *q;

    /* traverse list to find last node */
    for (q = NULL, p = *ppList; p; q = p, p = p->pNext) ;

    if (q)
    {
        q->pNext = pMsg;
    }
    else
    {
        *ppList = pMsg;
    }

    if (pMsg)
    {
        pMsg->pNext = NULL;
    }
    return pMsg;
}

/*
 * PktListExtract -- Pops/returns the first packet of a linked list
 */
static PKTNODE  *PktListExtract (PKTNODE **ppList)
{
    PKTNODE *pMsg;

    pMsg = *ppList;
    if (pMsg)
    {
        *ppList = pMsg->pNext;
    }
    return pMsg;
}

/*
 * PktListN -- Returns the number of packets on a linked list
 */
static int PktListN (PKTNODE *pList)
{
    int Cnt;

    for (Cnt = 0; pList; Cnt++, pList = pList->pNext);

    return Cnt;
}

static void TxStateChange(int channelIdx, TX_STATES newState)
{
    tp[channelIdx]->TxState = newState;
}

static void RxStateChange(int channelIdx, RX_STATES newState)
{
    rp[channelIdx]->RxState = newState;
}

static bool Is_UART_Rx_Error(int channelIdx)
{
    static unsigned int error_cnt[DESIP_NUM_CHANNELS] = {0};
    unsigned int        pre_error_cnt;
    bool                is_err=false;
    const int           hwLinkIdx = channelIdx;

    pre_error_cnt = VIBGetRxErrorCount (hwLinkIdx);

    if(pre_error_cnt != error_cnt[channelIdx])
    {
        is_err = true;
        ALOGI("HiSIP: UART Rx Error");
        error_cnt[channelIdx] = pre_error_cnt;
#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
        desip_stat[channelIdx].no_of_hw_err++;
#endif
    }
    return is_err;
}
#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
void SIP_Print_Desip_Staticstics(int channelIdx)
{
   printf("DESIP Stat ::\r\n");
   printf("  No. of Data Frame Xmit\t- %d\r\n", desip_stat[channelIdx].no_of_data_frame_xmit);
   printf("  No. of Cntrl Frame Xmit\t- %d\r\n", desip_stat[channelIdx].no_of_ctrl_frame_xmit);
   printf("  No. of Data Frame Recv\t- %d\r\n", desip_stat[channelIdx].no_of_data_frame_recv);
   printf("  No. of Cntrl Frame Recv\t- %d\r\n", desip_stat[channelIdx].no_of_ctrl_frame_recv);
   printf("  No. of NACK Xmited\t- %d\r\n", desip_stat[channelIdx].no_of_nack_frame_xmit);
   printf("  No of frame Re-Xmited\t- %d\r\n", desip_stat[channelIdx].no_of_frame_re_xmit);
   printf("  No of Checksum Error\t- %d\r\n", desip_stat[channelIdx].no_of_checksum_err);
   printf("  HW Error:\r\n");
   printf("  \tHW Fifo overflow+Parity Err+Framing Err\t- %d\r\n", desip_stat[channelIdx].no_of_hw_err);
}
#endif
