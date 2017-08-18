#include "Application_dataelement.h"
#include <stdio.h>

using namespace ApplicationDataElement;
using namespace autosar;

#define REC_SIGNAL  PassSeatMassgFct_info
#define SEND_SIGNAL MassgFctActv_info

int main(int argc, char** argv)
{
    DEReceiver<REC_SIGNAL> insig;
    DESender<SEND_SIGNAL>  outsig;

    printf("signalecho started\n");
    printf("Listening on %s, sending %s\n\n", REC_SIGNAL::Name(), SEND_SIGNAL::Name());

    insig.subscribe([&]() {
        const auto signal = insig.get();
        if (signal.isOk())
        {
            const auto  value    = signal.value().OnOff;
            const char* onoffstr = (value == OnOff1::On) ? "ON " : "OFF";
            printf("Received %s:%s ,  ", REC_SIGNAL::Name(), onoffstr);
            printf("sending %s:%s\n", SEND_SIGNAL::Name(), onoffstr);
            outsig.send({ value, value });
        }
        else
        {
            printf("Received %s with ERROR %d\n", REC_SIGNAL::Name(), signal.errorCode());
        }
    });

    sleep(1000000000); // sleep for ever (almost)

    return 0;
}
