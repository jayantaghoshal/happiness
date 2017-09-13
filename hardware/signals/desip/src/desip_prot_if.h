/* Copyright 2005 Delphi Delco Electronics */

#ifndef DESIP_PROT_IF_H
#define DESIP_PROT_IF_H

#define IS_SIPPROT_INSTRUMENTED_FOR_STAT (1)

typedef enum
{
    TX_FSM_EVENT_NORMAL_OP,
    TX_FSM_EVENT_SYNC,
    TX_FSM_EVENT_TXFORCE,
    TX_FSM_EVENT_TXACK,
    TX_FSM_EVENT_FREEBUF,
    TX_FSM_EVENT_NAK,
    TX_FSM_EVENT_SETFORCE,
    TX_FSM_EVENT_RXNACK,
    TX_FSM_EVENT_LAST
}
TX_FSM_EVENT;

void SIP_Rx_FSM (int channelIdx);
void SIP_Tx_FSM (TX_FSM_EVENT event, unsigned char c, int channelIdx);
int  SIPRecv (char *pBuf, int channelIdx);

#if defined(IS_SIPPROT_INSTRUMENTED_FOR_STAT)
void SIP_Print_Desip_Staticstics(int channelIdx);
#endif

#endif
