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
  srand(ss.getMyId() * getpid());
  m_timeout = Config::readPeriod;
  BOOST_LOG_TRIVIAL(info) << "Dispatcher starting";
  while(true){
    // Wait here for either timeout or a message from our beloved leader
    // some candidate that wants to get elected or some poor client
    // that thinks we are the leader.
    WakeupType wake = r.waitForMessage(m_timeout);
    switch(wake){
      case timedOut: {
          NodeState myState = ss.getNodeState();
          BOOST_LOG_TRIVIAL(info) << "Timeout in NodeState: " << myState;
          switch(myState){
            // Follower geting timeout will make him a Candidate
            case follower: {
                ss.setNodeState(candidate);  // Next state is candidate
                ss.setCandidateState(waitForTimeout);
                VoteCollector& vc = ss.getVoteCollector();
                vc.clearVotes();
                vc.storeVote(ss.getMyId());       // Vote for myself
                m_timeout = (Config::readPeriod * (rand() % 100)) / 100;
                BOOST_LOG_TRIVIAL(info) << "Candidate timeout: " << m_timeout;
              }
              break;
            // Candiate getting a timeout means nobody sent a RequestVote
            case candidate:
              switch(ss.getCandidateState()){
                case waitForTimeout:
                  ss.setTerm(ss.getTerm() + 1); // Increment my termId
                  m_timeout = Config::readPeriod;
                  ss.setCandidateState(waitForVotes);
                  m_s.sendRequestVote(ss);
                  break;
                case waitForVotes: // Timed out waiting for Votes
                  // Start another election
                  VoteCollector& vc = ss.getVoteCollector();
                  vc.clearVotes();
                  vc.storeVote(ss.getMyId());       // Vote for myself
                  m_timeout = (Config::readPeriod * (rand() % 100)) / 100;
                  BOOST_LOG_TRIVIAL(info) << "Candidate timeout: " << m_timeout;
                  ss.setCandidateState(waitForTimeout);
                  break;
              }
              break;
            // A leader should not experience timeouts (less he is in trouble)
            case leader:
              m_s.sendAppendEntries(ss);
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
  BOOST_LOG_TRIVIAL(info) << "Dispatcher ending";
}

void
Dispatcher::handleRequestVote(RequestVote& rv, ServerState& ss){
  int theirTerm = rv.getTerm();
  int nodeId = rv.getCandidateId();
  if(theirTerm < ss.getTerm()){
    m_s.sendVoteResponse(ss, nodeId, 0);
  } else {
    m_s.sendVoteResponse(ss, nodeId, 1);
    ss.setTerm(theirTerm);
    ss.setNodeState(follower);
  }
}

void
Dispatcher::handleVoteResponse(VoteResponse& vr, ServerState& ss){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a VoteResponse";
  if(vr.getVoteGranted() == 1) {
    VoteCollector& vc = ss.getVoteCollector();
    vc.storeVote(vr.getNodeId());
    if(vc.isElected()){
      BOOST_LOG_TRIVIAL(info) << "I am now the leader!";
      ss.setNodeState(leader);
      m_timeout = Config::leaderPeriod;
      m_s.sendAppendEntries(ss);
    }
  }
}

void
Dispatcher::handleAppendEntries(AppendEntries& ae, ServerState& ss){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a AppendEntries";
}

void
Dispatcher::handleClientMessage(){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a ClientMessage";
}
