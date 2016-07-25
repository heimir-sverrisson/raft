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
    HostEntry host_;
    UDPSocket sock_;
    bool run_;
    bool isReady_;
    Queue<AppendEntries> AppendEntriesQueue_;
    Queue<RequestVote> RequestVoteQueue_;
    Queue<VoteResponse> VoteResponseQueue_;
    int split(string& str, MessageType& mType, string& json);
    void setRun(bool s);
    mutex mtx_;
    mutex mtx2_;
    condition_variable cond_;
};

#endif
