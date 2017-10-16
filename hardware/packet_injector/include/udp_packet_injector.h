#include <arpa/inet.h>
#include <sys/socket.h>
#include <IDispatcher.h>
#include <ipcommandbus/Pdu.h>

using namespace Connectivity;

class UdpPacketInjector
{
public:
    UdpPacketInjector(uint32_t dst_port, uint32_t rd_port);


    void setup();
    void read();
    void CreateAndSendIpActivityMessage();
    void SendPdu(Pdu pdu);
    Pdu ReceivePdu();
    void StartActivityMessageTimer();
    void StopActivityMessageTimer();


    struct sockaddr_in sa,srcaddr,sa_out;
    socklen_t addrlen;
    int bcast_socket,read_socket, slen=sizeof(sa);
    uint32_t DST_PORT;      //Port to which to send data
    uint32_t RD_PORT;       //Port on which to listen to messages
    std::vector<uint8_t> buffer;
    std::uint8_t sequenceId_ = 0;

    tarmac::eventloop::IDispatcher &timer;
    tarmac::eventloop::IDispatcher::JobId activityPacketInjector;
};