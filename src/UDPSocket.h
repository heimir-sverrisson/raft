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

class UDPSocket {
  public:
    enum SocketType { server, client };
    UDPSocket(string server_name, string service, SocketType s_type);
    int receive(string& message, int max_size);
    void send(string message);
  private:
    SocketType my_type;
    int socket_info;
    struct addrinfo *results;
    void bind();
    void connect();
};

#endif
