#ifndef Receiver_h
#define Receiver_h

#include <Queue.h>
#include <HostEntry.h>
#include <MessageContent.h>
#include <UDPSocket.h>
#include <mutex>
#include <condition_variable>

using namespace std;

enum WakeupType {
  notRunning, timedOut, gotMessage
};

class Receiver{
  public:
    Receiver(HostEntry host);
    int getCount();
    MessageContent getMessage();
    void run();
    void stop();
    bool isRunning();
    WakeupType waitForMessage(int timeout);
  private:
    HostEntry host_;
    UDPSocket sock_;
    bool run_;
    bool isReady_;
    Queue<MessageContent> MessageContentQueue_;
    int split(string& str, MessageType& mType, string& json);
    void setRun(bool s);
    mutex mtx_;
    mutex mtx2_;
    condition_variable cond_;
};

#endif
