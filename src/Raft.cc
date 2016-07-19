#include <Raft.h>
#include <Dispatcher.h>
#include <boost/log/trivial.hpp>

Raft::Raft(string configFile, int myId)
  : m_ss(HostList(configFile, myId), myId),
    m_r(m_ss.getHostList().getHostById(myId))
{
  BOOST_LOG_TRIVIAL(info) << "Raft constructed";
}

void
Raft::run(){
  // Start a separate thread to receive messages
  m_rcv = thread(&Raft::receiveWorker, this);
  // Allow receiver thread to start
  this_thread::sleep_for(chrono::milliseconds(10));
  Dispatcher d;
  d.run(m_r, m_ss);
  m_r.stop(); // Stop the receiver thread
  m_rcv.join();
}

void
Raft::receiveWorker(){
  return m_r.run(); // This runs as a thread filling the queues
}
