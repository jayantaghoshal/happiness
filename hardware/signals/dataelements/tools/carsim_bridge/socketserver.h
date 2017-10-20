

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>

class socketserver
{
  private:
    int new_socket{-1};
    int valread{-1};
    const int PORT{8080};

    // Merge different part of the message to a single char*
    char * concat(const char * first, const char * second);

  
    const std::string SOF{"CarSim"};
    static constexpr std::uint8_t SOF_length{6};
    static constexpr std::uint8_t messageLength_length{4};//fixed 4 bytes describing an int32

    int getBodyMessageLengthFromHeader();
    std::string getBodyMessage(int messageLength);

  public: 
    void connect();
    std::string readMessage();
    void sendMessage(std::string message);
    
};