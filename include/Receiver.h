#ifndef Receiver_h
#define Receiver_h

#include <Queue.h>
#include <AppendEntries.h>
#include <RequestVote.h>
#include <VoteResponse.h>
#include <HostEntry.h>
#include <UDPSocket.h>
#include <mutex>
#include <condition_variable>

using namespace std;

enum MessageType {
  unknown, client, appendEntries, requestVote, voteResponse
};

enum WakeupType {
  notRunning, timedOut, gotMessage
};

class Receiver{
  public:
    Receiver(HostEntry host);
    int getCount(MessageType mType);
    AppendEntries getAppendEntries();
    RequestVote getRequestVote();
    VoteResponse getVoteResponse();
    void run();
    void stop();
    bool isRunning();
    WakeupType waitForMessage(int timeout);
  private:
    HostEntry m_host;
    UDPSocket m_sock;
    bool m_run;
    bool m_isReady;
    Queue<AppendEntries> m_AppendEntriesQueue;
    Queue<RequestVote> m_RequestVoteQueue;
    Queue<VoteResponse> m_VoteResponseQueue;
    int split(string& str, MessageType& mType, string& json);
    void setRun(bool s);
    mutex m_mtx;
    mutex m_mtx2;
    condition_variable m_cond;
};

#endif
