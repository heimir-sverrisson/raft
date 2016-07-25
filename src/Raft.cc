#include <Raft.h>
#include <Dispatcher.h>
#include <boost/log/trivial.hpp>

Raft::Raft(string configFile, int myId)
  : ss_(HostList(configFile, myId), myId),
    r_(ss_.getHostList().getHostById(myId))
{
  BOOST_LOG_TRIVIAL(info) << "Raft constructed";
}

void
Raft::run(){
  // Start a separate thread to receive messages
  rcv_ = thread(&Raft::receiveWorker, this);
  // Allow receiver thread to start
  this_thread::sleep_for(chrono::milliseconds(10));
  Dispatcher d;
  d.run(r_, ss_);
  r_.stop(); // Stop the receiver thread
  rcv_.join();
}

void
Raft::receiveWorker(){
  return r_.run(); // This runs as a thread filling the queues
}
