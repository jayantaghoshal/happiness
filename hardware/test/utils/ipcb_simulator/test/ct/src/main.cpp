#include <unistd.h>
#include <iostream>
#include "ipcb_simulator.h"
#include "LscMocker.h"

int main(void)
{
    LscMocker* lscMocker = new LscMocker();
    IpcbSimulator CyclicInjector(50000, 60000, 0);

    // lscMocker->RegisterLSC("iplmd-test");
    // lscMocker->RegisterLSC("iplmd-test1");
    // lscMocker->RegisterLSC("iplmd-test2");

    Pdu temp_pdu;
    CyclicInjector.StartActivityMessageTimer();
    while(1){
        temp_pdu = CyclicInjector.ReceivePdu();
        std::cout<<Pdu::toString(temp_pdu)<<std::endl;
        // CyclicInjector.SendPdu(temp_pdu);
    }
    return 0;
}


