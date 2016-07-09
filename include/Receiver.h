#ifndef Receiver_h
#define Receiver_h

#include <Queue.h>
#include <AppendEntries.h>
#include <RequestVote.h>
#include <HostEntry.h>
#include <UDPSocket.h>

enum MessageType{
  client, appendEntries, requestVote
};

class Receiver{
  public:
    Receiver(HostEntry host);
    int getCount(MessageType mType);
    AppendEntries getAppendEntries();
    RequestVote getRequestVote();
    void run();
    inline void stop() { m_run = false; };
  private:
    HostEntry m_host;
    UDPSocket m_sock;
    bool m_run;
    Queue<AppendEntries> m_AppendEntriesQueue;
    Queue<RequestVote> m_RequestVoteQueue;
};

#endif
