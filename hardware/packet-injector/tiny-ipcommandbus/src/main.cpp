#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <unistd.h>
#include "tiny-ipcommandbus/include/udp_packet_injector.h"

int main(void)
{
    UdpPacketInjector CyclicInjector(50000);
    CyclicInjector.StartActivityMessageTimer();
    return 0;
}


