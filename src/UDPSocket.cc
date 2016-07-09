#include <UDPSocket.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <poll.h>
#include <boost/log/trivial.hpp>  

UDPSocket::UDPSocket(string server_name, string service, SocketType s_type){
  struct addrinfo hints;
  my_type = s_type;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  int rv = getaddrinfo(server_name.c_str(), service.c_str(), 
                                        &hints, &results);
  if(rv != 0){
    BOOST_LOG_TRIVIAL(error) << "UDP get address info error: " << strerror(errno);
    throw strerror(errno);
  }
  switch(s_type){
    case SocketType::serverSocket: 
      bind();
      break;
    case SocketType::clientSocket:
      connect();
      break;
  }
}

void
UDPSocket::bind(){
  struct addrinfo *rp;
  for(rp = results; rp != NULL; rp = rp->ai_next){
    socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(socket_fd == -1)
      continue;
    if(::bind(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;
    close(socket_fd);
  }
  freeaddrinfo(results);
}

void
UDPSocket::connect(){
  struct addrinfo *rp;
  for(rp = results; rp != NULL; rp = rp->ai_next){
    socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(socket_fd == -1)
      continue;
    if(::connect(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;
  }
  freeaddrinfo(results);
}

int
UDPSocket::receive(string& message, int max_size, int timeout){
  int msg_length;
  char *msg = new char[max_size];
  memset(msg, 0, max_size);
  struct pollfd pdfs[1];

  if(timeout >= 0) { // Default is -1
    pdfs[0].fd = socket_fd;
    pdfs[0].events = POLLIN;
    poll(pdfs, 1, timeout);
    if((pdfs[0].revents & POLLIN) == 0){
      delete[] msg;
      return -1;
    }
  }
  msg_length = recv(socket_fd, msg, max_size, 0);
  if(msg_length < 0){
    BOOST_LOG_TRIVIAL(error) << "UDP recv error: " << strerror(errno);
    throw strerror(errno);
  }
  message = msg;
  delete[] msg;
  return msg_length;
}

void
UDPSocket::send(string message){
  if(::send(socket_fd, message.c_str(), message.length(), 0) < 0){
    BOOST_LOG_TRIVIAL(error) << "UDP send error: " << strerror(errno);
    throw strerror(errno);
  }
}
