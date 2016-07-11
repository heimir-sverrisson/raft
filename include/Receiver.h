#ifndef Receiver_h
#define Receiver_h

#include <Queue.h>
#include <AppendEntries.h>
#include <RequestVote.h>
#include <HostEntry.h>
#include <UDPSocket.h>
#include <mutex>
#include <condition_variable>

using namespace std;

enum MessageType{
  unknown = -1, client = 0, appendEntries = 1, requestVote = 2
};

class Receiver{
  public:
    Receiver(HostEntry host);
    int getCount(MessageType mType);
    AppendEntries getAppendEntries();
    RequestVote getRequestVote();
    void run();
    inline void stop() { m_run = false; };
    void waitForMessage(); // Blocks until new message
  private:
    HostEntry m_host;
    UDPSocket m_sock;
    bool m_run;
    bool m_isReady;
    Queue<AppendEntries> m_AppendEntriesQueue;
    Queue<RequestVote> m_RequestVoteQueue;
    int split(string& str, MessageType& mType, string& json);
    mutex m_mtx;
    condition_variable m_cond;
};

#endif
