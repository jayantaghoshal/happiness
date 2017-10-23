#include <arpa/inet.h>
#include <sys/socket.h>
#include <IDispatcher.h>
#include "ipcommandbus/Pdu.h"

using namespace Connectivity;

class IpcbSimulator
{
public:
    IpcbSimulator(uint32_t dst_port,uint32_t src_port, int bcast_enable);


    void setup();
    void read();
    void CreateAndSendIpActivityMessage();
    void SendPdu(Pdu pdu);
    Pdu ReceivePdu();
    void StartActivityMessageTimer();
    void StopActivityMessageTimer();


    struct sockaddr_in sa,srcaddr,sa_out;
    socklen_t addrlen;
    int local_socket,read_socket, slen=sizeof(sa);
    int broadcastEnable;
    std::string SEND_ADDR;
    uint32_t DST_PORT;      //Port to which to send data
    uint32_t SRC_PORT;   //The default port from which to send data
    std::vector<uint8_t> buffer;
    std::uint8_t sequenceId_ = 0;

    tarmac::eventloop::IDispatcher &timer;
    tarmac::eventloop::IDispatcher::JobId activityPacketInjector;
};