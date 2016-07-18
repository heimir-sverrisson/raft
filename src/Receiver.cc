#include <Receiver.h>
#include <Config.h>
#include <string>
#include <boost/log/trivial.hpp>
#include <chrono>

#include <iostream>

Receiver::Receiver(HostEntry host)
  : m_host(host),
  m_sock(host.getHost(), host.getService(), SocketType::serverSocket),
  m_run(false), m_isReady(false)
{}

void 
Receiver::setRun(bool s){
  unique_lock<mutex> lk(m_mtx2);
  m_run = s;
}

bool
Receiver::isRunning(){
  unique_lock<mutex> lk(m_mtx2);
  return m_run;
}

void
Receiver::stop(){
  setRun(false);
}

void
Receiver::run(){
  BOOST_LOG_TRIVIAL(info) << "Receiver started";
  setRun(true);
  while(isRunning()) {
    string str;
    int ret = m_sock.receive(str, Config::maxMessageSize);
    MessageType mType;
    string json;
    ret = split(str, mType, json);
    if(ret == 0){
      switch(mType){
        case appendEntries:
          {
            AppendEntries ae;
            ae.parse_json(json);
            m_AppendEntriesQueue.push(ae);
          }
          break;
        case requestVote:
          {
            RequestVote rv;
            rv.parse_json(json);
            m_RequestVoteQueue.push(rv);
          }
          break;
        case voteResponse:
          {
            VoteResponse vr;
            vr.parse_json(json);
            m_VoteResponseQueue.push(vr);
          }
          break;
        case client:
        default:
          BOOST_LOG_TRIVIAL(error) << "Unknown MessageType: " << str;
      }
      // Set condition if we got a message
      {
        unique_lock<mutex> lk(m_mtx);
        m_isReady = true;
        lk.unlock();
        m_cond.notify_one();
      }
    } else {
      BOOST_LOG_TRIVIAL(error) << "Could not split: " << str;
    }
  }
  BOOST_LOG_TRIVIAL(info) << "Receiver stopped";
}

WakeupType
Receiver::waitForMessage(int timeout){
  if(m_AppendEntriesQueue.size() > 0 ||
     m_RequestVoteQueue.size() > 0   ||
     m_VoteResponseQueue.size() > 0){
    return gotMessage;
  }
  if(!isRunning()){
    return notRunning;
  }
  unique_lock<mutex> lk(m_mtx);
  m_isReady = false;
  auto now = std::chrono::system_clock::now();
  auto until = now + std::chrono::milliseconds(timeout);
  if(m_cond.wait_until(lk, until) == cv_status::timeout)
    return timedOut;
  else
    return gotMessage;
}

int
Receiver::split(string& str, MessageType& mType, string& json){
  size_t sepPos = str.find_first_of(Config::messageSeparator);
  if(sepPos == string::npos){
    BOOST_LOG_TRIVIAL(error) << "No separator found in: " << str;
    return -1;
  }
  int intEnum = stoi(str.substr(0, sepPos));
  mType = (MessageType) intEnum;
  json = str.substr(sepPos + 1, str.length() - sepPos - 1);
  return 0;
}

int
Receiver::getCount(MessageType mType){
  switch(mType){
    case appendEntries:
      return m_AppendEntriesQueue.size();
    case requestVote:
      return m_RequestVoteQueue.size();
    case voteResponse:
      return m_VoteResponseQueue.size();
    case client:
      return 0;
    default:
      string str = "Illegal MessageType: " + to_string(mType);
      BOOST_LOG_TRIVIAL(error) << str;
      return 0;
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

VoteResponse
Receiver::getVoteResponse(){
  return m_VoteResponseQueue.pop();
}
