#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <UDPSocket.h>
#include <AppendEntries.h>
#include <raft_util.h>
#include <boost/log/trivial.hpp>
#include <KeyValueStore.h>
#include <IdStore.h>
#include <Receiver.h>
#include <Config.h>


void send_ae_msg(vector<AppendEntries> aes){
  BOOST_LOG_TRIVIAL(info) << "Starting thread send_aes_msg()";
  char sep = Config::messageSeparator;
  try{
    UDPSocket sock = UDPSocket("localhost", "2001", SocketType::clientSocket);
    for(auto& ae : aes){
      string s = to_string(MessageType::appendEntries) + sep + ae.to_string();
      sock.send(s);
      this_thread::sleep_for(chrono::milliseconds(1000));
    }
  } catch(const char *msg) {
    cout << "Error message: " << msg << endl;
  }
}

void send_rv_msg(vector<RequestVote> rvs){
  BOOST_LOG_TRIVIAL(info) << "Starting thread send_rv_msg()";
  char sep = Config::messageSeparator;
  try{
    this_thread::sleep_for(chrono::milliseconds(300));
    UDPSocket sock = UDPSocket("localhost", "2001", SocketType::clientSocket);
    for(auto& rv : rvs){
      string s = to_string(MessageType::requestVote) + sep + rv.to_string();
      sock.send(s);
      this_thread::sleep_for(chrono::milliseconds(1000));
    }
  } catch(const char *msg) {
    cout << "Error message: " << msg << endl;
  }
}

AppendEntries make_entry(int i){
  AppendEntries ae(i + 1, i + 2, i + 3, i + 4, i + 5);
  using namespace rapidjson;
  for(int j=0; j < 10; j++){
    Value v(10 - j + i);
    ae.addEntry(v);
  }
  return ae;
}

RequestVote make_vote(int i){
  RequestVote rv(i + 1, i + 2, i + 3, i + 4);
  return rv;
}

void dummy(Receiver *r){
  return r->run();
}

int main(int argc, char *arg[]) {
  log_init();

  Receiver r(HostEntry(1, "localhost", "2001"));

  BOOST_LOG_TRIVIAL(info) << "Starting main()";
  vector<AppendEntries> aes;
  aes.push_back(make_entry(0));
  aes.push_back(make_entry(2));
  aes.push_back(make_entry(4));
  aes.push_back(make_entry(5));
  aes.push_back(make_entry(7));
  vector<RequestVote> rvs;
  rvs.push_back(make_vote(0));
  rvs.push_back(make_vote(10));
  rvs.push_back(make_vote(11));
  rvs.push_back(make_vote(13));
  rvs.push_back(make_vote(17));

  thread rcv(&dummy, &r);
  thread snd_1(&send_ae_msg, aes);
  thread snd_2(&send_rv_msg, rvs);

  int total_count = aes.size() + rvs.size();
  int count = 0;
  do{
    r.waitForMessage();
    // Deplete all queues starting with RequestVoteQueue
    while(r.getCount(MessageType::requestVote) > 0){
      RequestVote rv = r.getRequestVote();
      cout << rv.to_string() << endl;
      count++;
    }
    while(r.getCount(MessageType::appendEntries) > 0){
      AppendEntries ae = r.getAppendEntries();
      cout << ae.to_string() << endl;
      count++;
    }
  } while(count < total_count);
  r.stop();
  snd_1.join();
  snd_2.join();
  rcv.join();
}
