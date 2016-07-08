#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <UDPSocket.h>
#include <AppendEntries.h>
#include <raft_util.h>
#include <boost/log/trivial.hpp>
#include <KeyValueStore.h>


bool done = false;


void receive_msg(void){
  string str;
  AppendEntries ae;
  BOOST_LOG_TRIVIAL(info) << "Starting thread receive_msg()";
  UDPSocket s = UDPSocket("localhost", "2001", 
                        UDPSocket::SocketType::server);
  while(!done){
    int ret = s.receive(str, 1000, 1100);
    if(ret < 0){
      BOOST_LOG_TRIVIAL(info) << "Timeout!";
      return;
    }
    if(str.length() == 0){
      BOOST_LOG_TRIVIAL(info) << "Done!";
      return;
    }
    ae.parse_json(str);
    cout << str << endl;
    string the_json = ae.to_string();
    cout << the_json << endl;
    the_json = ae.get_entries();
    cout << the_json << endl;
  }
}

void send_msg(vector<AppendEntries> aes){
  BOOST_LOG_TRIVIAL(info) << "Starting thread send_msg()";
  try{
    UDPSocket sock = UDPSocket("localhost", "2001", 
                          UDPSocket::SocketType::client);
    for(auto& ae : aes){
      string s = ae.to_string();
      sock.send(s);
      this_thread::sleep_for(chrono::milliseconds(1000));
    }
    done = true;
    sock.send("");
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

int main(int argc, char *arg[]) {
  log_init();

  BOOST_LOG_TRIVIAL(info) << "Starting main()";
  vector<AppendEntries> aes;
  aes.push_back(make_entry(0));
  aes.push_back(make_entry(2));
  aes.push_back(make_entry(4));

  thread rcv(&receive_msg);
  thread snd(&send_msg, aes);
  snd.join();
  rcv.join();
  KeyValueStore s;
  s.setValue("key something","value something");
  s.setValue("key else","value else");
  s.dump();
  cout << s.getValue("nonexistant key").length() << endl;
}
