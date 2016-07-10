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
    MessageType mType;
    string json;
    ret = split(str, mType, json);
    if(ret == 0){
      switch(mType){
        case MessageType::appendEntries:
          {
            AppendEntries ae;
            ae.parse_json(json);
            m_AppendEntriesQueue.push(ae);
          }
          break;
        default:
          BOOST_LOG_TRIVIAL(error) << "Unknown MessageType: " << str;
      }
    } else {
      BOOST_LOG_TRIVIAL(error) << "Could not split: " << str;
    }
  }
}

int
Receiver::split(string& str, MessageType& mType, string& json){
  size_t sepPos = str.find_first_of(Config::messageSeparator);
  if(sepPos == string::npos){
    BOOST_LOG_TRIVIAL(error) << "No separator found in: " << str;
    return -1;
  }
  int intEnum = stoi(str.substr(0, sepPos));
  switch(intEnum){
    case 0: 
      mType = MessageType::client;
      break;
    case 1:
      mType = MessageType::appendEntries;
      break;
    case 2:
      mType = MessageType::appendEntries;
      break;
    default:
      mType = MessageType::unknown;
  }
  json = str.substr(sepPos + 1, str.length() - sepPos - 1);
  return 0;
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
