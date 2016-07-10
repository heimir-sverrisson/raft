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


void send_msg(vector<AppendEntries> aes){
  BOOST_LOG_TRIVIAL(info) << "Starting thread send_msg()";
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

AppendEntries make_entry(int i){
  AppendEntries ae(i + 1, i + 2, i + 3, i + 4, i + 5);
  using namespace rapidjson;
  for(int j=0; j < 10; j++){
    Value v(10 - j + i);
    ae.addEntry(v);
  }
  return ae;
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

  thread rcv(&dummy, &r);
  thread snd(&send_msg, aes);
  this_thread::sleep_for(chrono::milliseconds(aes.size() *1000 + 500));
  r.stop();
  snd.join();
  rcv.join();
  while(r.getCount(MessageType::appendEntries) > 0){
    AppendEntries ae = r.getAppendEntries();
    cout << ae.to_string() << endl;
  }
}
