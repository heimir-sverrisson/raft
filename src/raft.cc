#include <iostream>
#include <UDPSocket.h>
#include <AppendEntries.h>
#include <cstring>
#include <thread>

using namespace std;

bool done = false;

void receive_msg(void){
  string str;
  UDPSocket s = UDPSocket("localhost", "2001", 
                        UDPSocket::SocketType::server);
  while(!done){
    int ret = s.receive(str, 100, 1002);
    if(ret < 0)
      return;
    cout << str << endl;
  }
}

void send_msg(void){
  string msgs[] = { "Halló", "Bless" };

  try{
    UDPSocket sock = UDPSocket("localhost", "2001", 
                          UDPSocket::SocketType::client);
    for(string s : msgs){
      sock.send(s);
      this_thread::sleep_for(chrono::milliseconds(1000));
    }
    done = true;
    sock.send("");
  } catch(const char *msg) {
    cout << "Error message: " << msg << endl;
  }
}

int main(int argc, char *arg[]) {
  thread rcv(&receive_msg);
  thread snd(&send_msg);
  snd.join();
  rcv.join();
  AppendEntries ae(1, 2, 3, 4, 5);
  using namespace rapidjson;
  for(int i=0; i < 10; i++){
    Value v(10 - i);
    ae.addEntry(v);
  }
  char the_json[1000];
  strcpy(the_json, ae.to_string());
  AppendEntries aee;
  aee.parse_json(the_json);
  using namespace std;
  cout << the_json << endl << aee.get_entries() << endl;
}
