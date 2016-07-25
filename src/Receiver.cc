#include <Receiver.h>
#include <Config.h>
#include <string>
#include <boost/log/trivial.hpp>
#include <chrono>

#include <iostream>

Receiver::Receiver(HostEntry host)
  : host_(host),
  sock_(host.getHost(), host.getService(), SocketType::serverSocket),
  run_(false), isReady_(false)
{}

void 
Receiver::setRun(bool s){
  unique_lock<mutex> lk(mtx2_);
  run_ = s;
}

bool
Receiver::isRunning(){
  unique_lock<mutex> lk(mtx2_);
  return run_;
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
    int ret = sock_.receive(str, Config::maxMessageSize, 2000);
    if(ret < 0) // Handle timeout
      continue;
    MessageType mType;
    string json;
    ret = split(str, mType, json);
    if(ret == 0){
      switch(mType){
        case appendEntries:
          {
            AppendEntries ae;
            ae.parse_json(json);
            AppendEntriesQueue_.push(ae);
          }
          break;
        case requestVote:
          {
            RequestVote rv;
            rv.parse_json(json);
            RequestVoteQueue_.push(rv);
          }
          break;
        case voteResponse:
          {
            VoteResponse vr;
            vr.parse_json(json);
            VoteResponseQueue_.push(vr);
          }
          break;
        case client:
        default:
          BOOST_LOG_TRIVIAL(error) << "Unknown MessageType: " << str;
      }
      // Set condition if we got a message
      {
        unique_lock<mutex> lk(mtx_);
        isReady_ = true;
        lk.unlock();
        cond_.notify_one();
      }
    } else {
      BOOST_LOG_TRIVIAL(error) << "Could not split: " << str;
    }
  }
  BOOST_LOG_TRIVIAL(info) << "Receiver stopped";
}

WakeupType
Receiver::waitForMessage(int timeout){
  if(AppendEntriesQueue_.size() > 0 ||
     RequestVoteQueue_.size() > 0   ||
     VoteResponseQueue_.size() > 0){
    return gotMessage;
  }
  if(!isRunning()){
    return notRunning;
  }
  unique_lock<mutex> lk(mtx_);
  isReady_ = false;
  auto now = std::chrono::system_clock::now();
  auto until = now + std::chrono::milliseconds(timeout);
  if(cond_.wait_until(lk, until) == cv_status::timeout)
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
      return AppendEntriesQueue_.size();
    case requestVote:
      return RequestVoteQueue_.size();
    case voteResponse:
      return VoteResponseQueue_.size();
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
  return AppendEntriesQueue_.pop();
}

RequestVote
Receiver::getRequestVote(){
  return RequestVoteQueue_.pop();
}

VoteResponse
Receiver::getVoteResponse(){
  return VoteResponseQueue_.pop();
}
