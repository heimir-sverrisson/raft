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
    UDPSocket(string server_name, string service, SocketType s_type);
    int receive(string& message, int max_size, int timeout = -1);
    void send(string message);
    void close();
    ~UDPSocket();
  private:
    SocketType my_type;
    int socket_fd;
    struct addrinfo *results;
    void bind();
    void connect();
};

#endif
