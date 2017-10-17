#include <unistd.h>
#include <iostream>
#include "udp_packet_injector.h"
#include "LscMocker.h"

int main(void)
{
    LscMocker* lscMocker = new LscMocker();
    UdpPacketInjector CyclicInjector(50000, 70000);

    lscMocker->RegisterLSC("iplmd-test");
    lscMocker->RegisterLSC("iplmd-test1");
    lscMocker->RegisterLSC("iplmd-test2");

    Pdu temp_pdu;

    while(1){
        temp_pdu = CyclicInjector.ReceivePdu();
        std::cout<<Pdu::toString(temp_pdu)<<std::endl;
    }
    return 0;
}


