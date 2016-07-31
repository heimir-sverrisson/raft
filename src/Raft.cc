#include <Raft.h>
#include <Dispatcher.h>
#include <boost/log/trivial.hpp>
#include <RaftProtocol.h>

Raft::Raft(string configFile, int myId)
  : ss_(HostList(configFile, myId), myId),
    r_(ss_.getHostList().getHostById(myId))
{
  BOOST_LOG_TRIVIAL(info) << "Raft constructed";
}

using namespace raft_fsm;
void
Raft::run(){
  // Start a separate thread to receive messages
  rcv_ = thread(&Raft::receiveWorker, this);
  // Allow receiver thread to start
  this_thread::sleep_for(chrono::milliseconds(10));
  // Dispatcher d;
  // d.run(r_, ss_);
  raft_fsm::run(r_, ss_);
  // RaftProtocol rp();
  r_.stop(); // Stop the receiver thread
  rcv_.join();
}

void
Raft::receiveWorker(){
  return r_.run(); // This runs as a thread filling the queues
}
