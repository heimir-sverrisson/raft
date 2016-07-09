#include <Receiver.h>
#include <Config.h>
#include <string>
#include <boost/log/trivial.hpp>
#include <iostream>

Receiver::Receiver(HostEntry host)
  : m_host(host),
  m_sock(host.getHost(), host.getService(), SocketType::serverSocket),
  m_run(true)
{}

void
Receiver::run(){
  int readCount = 0;
  while(m_run){
    string str;
    int ret = m_sock.receive(str, Config::maxMessageSize, Config::readPeriod);
    if(ret < 0){ // Timeout
      if((readCount++ % 10) == 0)
        BOOST_LOG_TRIVIAL(info) << "Tick";
        continue;
    }
    cout << str << endl;
  }
}

int
Receiver::getCount(MessageType mType){
  switch(mType){
    case MessageType::appendEntries:
      return m_AppendEntriesQueue.size();
    case MessageType::requestVote:
      return m_RequestVoteQueue.size();
    default:
      throw("Illegal MessageType");
  }
}

AppendEntries
Receiver::getAppendEntries(){
  return m_AppendEntriesQueue.pop();
}

RequestVote
Receiver::getRequestVote(){
  return m_RequestVoteQueue.pop();
}
