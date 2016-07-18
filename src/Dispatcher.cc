#include <Dispatcher.h>
#include <boost/log/trivial.hpp>
#include <RequestVote.h>
#include <AppendEntries.h>
#include <Config.h>

void
Dispatcher::run(Receiver& r, ServerState& ss){
  // Receiver must already be running at this point
  if(!r.isRunning()){
    string str = "Receiver was not running!";
    BOOST_LOG_TRIVIAL(error) << str;
    throw(str);
  }
  int timeout = Config::readPeriod;
  while(true){
    // Wait here for either timeout or a message from our beloved leader
    // some candidate that wants to get elected or some poor client
    // that thinks we are the leader.
    WakeupType wake = r.waitForMessage(timeout);
    switch(wake){
      case timedOut: {
          NodeState myState = ss.getNodeState();
          switch(myState){
            // Follower geting timeout will make him a Candidate
            case follower:
              ss.setTerm(ss.getTerm() + 1); // Increment my termId
              ss.setNodeState(candidate);       // And become a candidate
              break;
            // Candiate getting a timeout will have him stay Candidate
            case candidate:
            // A leader does not care for timeouts
            case leader:
              break;
            default:
              BOOST_LOG_TRIVIAL(error) << "Illegal State: " << myState;
          }
        }
        break;
      case gotMessage:
        while(r.getCount(MessageType::requestVote) > 0){
          RequestVote rv = r.getRequestVote();
          BOOST_LOG_TRIVIAL(info) << rv.to_string();
          handleRequestVote(rv, ss);
        }
        while(r.getCount(MessageType::appendEntries) > 0){
          AppendEntries ae = r.getAppendEntries();
          BOOST_LOG_TRIVIAL(info) << ae.to_string();
          handleAppendEntries(ae, ss);
        }
        while(r.getCount(voteResponse) > 0){
          VoteResponse vr = r.getVoteResponse();
          BOOST_LOG_TRIVIAL(info) << vr.to_string();
          handleVoteResponse(vr, ss);
        }
        while(r.getCount(MessageType::client) > 0){
          BOOST_LOG_TRIVIAL(info) << "Got a stray client message!";
          handleClientMessage();
        }
        break;
      case notRunning:
        BOOST_LOG_TRIVIAL(error) << "Receiver is dead!";
        throw("Receiver is dead!");
      default:
        BOOST_LOG_TRIVIAL(error) << "Illegal WakeupType: " << wake;
    } // switch(wake)
  } // while(true);
}

void
Dispatcher::handleRequestVote(RequestVote& rv, ServerState& ss){
  int theirTerm = rv.get_term();
  if(theirTerm <= ss.getTerm()){
    sendVoteResponse(rv, ss, 0);
  } else {
    sendVoteResponse(rv, ss, 1);
    ss.setNodeState(follower);
    ss.setTerm(theirTerm);
  }
}

void
Dispatcher::sendVoteResponse(RequestVote& rv, ServerState& ss, int vote){
  char sep = Config::messageSeparator;
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Sending VoteResponse";
  int candidate = rv.get_candidateId();
  HostEntry h = ss.getHostList().getHostById(candidate);
  UDPSocket sock(h.getHost(), h.getService(), clientSocket);
  VoteResponse vr(ss.getMyId(), ss.getTerm(), 0);
  string s = to_string(MessageType::voteResponse) + sep + vr.to_string();
  sock.send(s);
  sock.close();
}

void
Dispatcher::handleVoteResponse(VoteResponse& vr, ServerState& ss){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a VoteResponse";
}

void
Dispatcher::handleAppendEntries(AppendEntries& ae, ServerState& ss){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a AppendEntries";
}

void
Dispatcher::handleClientMessage(){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a ClientMessage";
}
