#include <UDPSocket.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <errno.h>

UDPSocket::UDPSocket(string server_name, string service, SocketType s_type){
  struct addrinfo hints;
  my_type = s_type;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  int rv = getaddrinfo(server_name.c_str(), service.c_str(), 
                                        &hints, &results);
  if(rv != 0){
    throw strerror(errno);
  }
  switch(s_type){
    case SocketType::server: 
      bind();
      break;
    case SocketType::client:
      connect();
      break;
  }
}

void
UDPSocket::bind(){
  struct addrinfo *rp;
  for(rp = results; rp != NULL; rp = rp->ai_next){
    socket_info = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(socket_info == -1)
      continue;
    if(::bind(socket_info, rp->ai_addr, rp->ai_addrlen) == 0)
      break;
    close(socket_info);
  }
  freeaddrinfo(results);
}

void
UDPSocket::connect(){
  struct addrinfo *rp;
  for(rp = results; rp != NULL; rp = rp->ai_next){
    socket_info = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(socket_info == -1)
      continue;
    if(::connect(socket_info, rp->ai_addr, rp->ai_addrlen) == 0)
      break;
  }
  freeaddrinfo(results);
}

int
UDPSocket::receive(string& message, int max_size){
  int msg_length;
  char *msg = new char[max_size];
  msg_length = recv(socket_info, msg, max_size, 0);
  if(msg_length < 0){
    throw strerror(errno);
  }
  message = msg;
  delete[] msg;
  return msg_length;
}

void
UDPSocket::send(string message){
  if(::send(socket_info, message.c_str(), message.length(), 0) < 0){
    throw strerror(errno);
  }
}
