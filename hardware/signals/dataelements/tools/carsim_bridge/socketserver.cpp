#include "socketserver.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <vector>


  void socketserver::connect()
  {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    printf("DEBUG-1\n");
        
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
      throw std::runtime_error("socket failed");
    }

    printf("DEBUG-2\n");
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        throw std::runtime_error("setsockopt");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    printf("DEBUG-3\n");
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                sizeof(address))<0)
    {
        throw std::runtime_error("bind failed");
    }
    if (listen(server_fd, 3) < 0)
    {
        throw std::runtime_error("listen");
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                      (socklen_t*)&addrlen))<0)
    {
        throw std::runtime_error("accept");
    }

    printf("DEBUG-4\n");

    
    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("Hello message sent\n"); 
  }

  std::string socketserver::readMessage()
  {
    const int messageLength = getBodyMessageLengthFromHeader();

    if(messageLength > 0)
      return getBodyMessage(messageLength);
    else
      return "-";
  }

  void socketserver::sendMessage(std::string message)
  {
    //char header[4 + 4 + 1] = "";
    //sprintf(header, "%s%4lu", SOF.c_str(), message.size());
    //char const *completeMessage = concat(header, message.c_str());

    char completeMessage[SOF_length + messageLength_length + message.size()];
    //sprintf(completeMessage, "%s|%4lu|%s", SOF.c_str(), message.size(), message.c_str());
    sprintf(completeMessage, "%s%04d%s", SOF.c_str(), int(message.size()), message.c_str());
    printf("Send:[%s]\n", completeMessage);

    //printf("DEBUG-concat:[%s][%s] (total:%zd)\n", header, message.c_str(), strlen(completeMessage));

    size_t s = send(new_socket , completeMessage , strlen(completeMessage), 0 );
    
    printf("===========\n\n");
    //send_string(new_socket, completeMessage);
  }

  char * socketserver::concat(const char * first, const char * second)
  {
      int lf = strlen(first);
      int ls = strlen(second);
      int len = lf + ls;
      char * rb = new char[len+1];
      memcpy(rb, first, lf);
      memcpy(rb+lf, second, ls);
      rb[len] = 0;
      return rb;
  }

  int socketserver::getBodyMessageLengthFromHeader()
  {
    char headerBuffer[SOF_length + messageLength_length + 1] = {0};//+1 for nullterminator
    
    int numberOfBytesRead = read(new_socket, headerBuffer, SOF_length + messageLength_length);//Read header
    //printf("h-buffer:%s (number of bytes: %zd)\n", headerBuffer, numberOfBytesRead);

    // Check header message lenght
    if(numberOfBytesRead != SOF_length + messageLength_length)
    {
      // Should instead difference between exception and no message received
      return 0;
      //throw std::runtime_error("MessageHeader too short");
    }

    // Check SOF
    std::string bufferSOF(headerBuffer, SOF_length);
    if (bufferSOF != SOF)
    {
      std::string err = "Incorrect SOF, expected " + SOF + ", received " + bufferSOF;
      throw std::runtime_error(err);
    }

    // Check message body length
    std::string messageLengthStr(headerBuffer + SOF_length, messageLength_length);

    printf("DEBUG-Header:[%s][%s]\n", bufferSOF.c_str(), messageLengthStr.c_str()); 

    return atoi(messageLengthStr.c_str());
  }

std::string socketserver::getBodyMessage(int messageLength)
{
  std::vector<char> messageBuffer(messageLength, 0);
  int numberOfBytesRead = read(new_socket, messageBuffer.data(), messageLength);

  if(numberOfBytesRead < 0)
  throw std::runtime_error("Erik stuff");

  //printf("b-buffer:%.*s (number of bytes: %zd)\n", numberOfBytesRead, messageBuffer.data(), numberOfBytesRead);

  if(numberOfBytesRead != messageLength)
  {
    throw std::runtime_error("Wrong message length or error in read");
  }

  std::string teststuff(messageBuffer.data(), numberOfBytesRead);

  printf("DEBUG-Body  :[%s](bytes: %zd)\n", teststuff.c_str(), numberOfBytesRead); 

  return teststuff;
}