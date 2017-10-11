#define LOG_TAG "packet_injector"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <IDispatcher.h>
#include "tiny-ipcommandbus/include/Pdu.h"

using namespace Connectivity;

class UdpPacketInjector
{
public:
    UdpPacketInjector(uint16_t port);


    void setup();
    void read();
    // void send(const char *msg);
    void CreateAndSendIpActivityMessage();
    void SendPdu(Pdu pdu);
    void ReceivePdu(Pdu &pdu);
    void StartActivityMessageTimer();
    void StopActivityMessageTimer();


    struct sockaddr_in sa,srcaddr;
    int src_socket,slen=sizeof(sa);
    uint16_t DST_PORT;
    std::vector<uint8_t> buffer;
    std::uint8_t sequenceId_ = 0;

    tarmac::eventloop::IDispatcher &timer;
    tarmac::eventloop::IDispatcher::JobId activityPacketInjector;
};