#ifndef UDPSocket_h
#define UDPSocket_h

#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <sys/types.h>
#include <netdb.h>

using namespace std;

enum SocketType { serverSocket, clientSocket };

class UDPSocket {
  public:
    UDPSocket(string server_name, string service, SocketType socketType);
    int receive(string& message, int max_size, int timeout = -1);
    void send(string message);
    void close();
    ~UDPSocket();
  private:
    SocketType m_socketType;
    int m_socketFd;
    struct addrinfo *m_results;
    void bind();
    void connect();
};

#endif
