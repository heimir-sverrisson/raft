#include <iostream>
#include <UDPSocket.h>
#include <cstring>
#include <thread>

using namespace std;

bool done = false;

void receive_msg(void){
  string str;
  UDPSocket s = UDPSocket("localhost", "2001", 
                        UDPSocket::SocketType::server);
  while(!done){
    s.receive(str, 100);
    cout << str << endl;
  }
}

void send_msg(void){
  string msgs[] = { "Halló", "Bless", "Hvað", "Svo" };

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
}
