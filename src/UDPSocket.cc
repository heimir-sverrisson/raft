#include <UDPSocket.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <poll.h>
#include <boost/log/trivial.hpp>

UDPSocket::UDPSocket(string server_name, string service,
                     SocketType socketType) {
    struct addrinfo hints;
    socketType_ = socketType;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    int rv = 0;
    for (int i = 0; i < 3; i++) {
        rv = getaddrinfo(server_name.c_str(), service.c_str(), &hints,
                         &results_);
        if (rv == 0) {
            break;
        } else {
            BOOST_LOG_TRIVIAL(info) << "UDP get address info error: " << rv
                                    << ", " << strerror(errno) << ". Retrying";
        }
        usleep(20000);  // Wait 20 msec for next try
    }
    if (rv != 0) {
        BOOST_LOG_TRIVIAL(error) << "UDP get address info error: " << rv << ", "
                                 << strerror(errno);
        throw strerror(errno);
    }

    switch (socketType) {
        case SocketType::serverSocket:
            bind();
            break;
        case SocketType::clientSocket:
            connect();
            break;
    }
}

UDPSocket::~UDPSocket() { close(); }

void UDPSocket::bind() {
    struct addrinfo *rp;
    for (rp = results_; rp != NULL; rp = rp->ai_next) {
        socketFd_ = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socketFd_ == -1) continue;
        if (::bind(socketFd_, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        } else {
            BOOST_LOG_TRIVIAL(error) << "UDP bind error: " << strerror(errno);
        }
        ::close(socketFd_);
    }
    freeaddrinfo(results_);
}

void UDPSocket::connect() {
    struct addrinfo *rp;
    for (rp = results_; rp != NULL; rp = rp->ai_next) {
        socketFd_ = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socketFd_ == -1) {
            continue;
        }
        if (::connect(socketFd_, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        } else {
            BOOST_LOG_TRIVIAL(error)
                << "UDP connect error: " << strerror(errno);
        }
    }
    freeaddrinfo(results_);
}

int UDPSocket::receive(string &message, int max_size, int timeout) {
    int msg_length;
    struct pollfd pdfs[1];
    memset(pdfs, 0, sizeof(pdfs));

    if (timeout >= 0) {  // Default is -1
        pdfs[0].fd = socketFd_;
        pdfs[0].events = POLLIN;
        poll(pdfs, 1, timeout);
        if ((pdfs[0].revents & POLLIN) == 0) {
            return -1;
        }
    }
    // We are here because we have received a datagram
    char *msg = new char[max_size];
    memset(msg, 0, max_size);
    msg_length = recv(socketFd_, msg, max_size, 0);
    if (msg_length < 0) {
        BOOST_LOG_TRIVIAL(error) << "UDP recv error: " << strerror(errno);
        throw strerror(errno);
    }
    message.clear();
    message.append(msg);
    delete[] msg;
    return msg_length;
}

void UDPSocket::close() {
    if (socketFd_ != -1) {
        if (::close(socketFd_) < 0) {
            BOOST_LOG_TRIVIAL(error)
                << "UDP close socket error: " << strerror(errno);
        }
        socketFd_ = -1;
    }
}

void UDPSocket::send(string message) {
    if (::send(socketFd_, message.c_str(), message.length(), 0) < 0) {
        BOOST_LOG_TRIVIAL(error) << "UDP send error: " << strerror(errno);
        throw strerror(errno);
    }
}
