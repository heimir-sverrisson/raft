#include <iostream>
#include <UDPSocket.h>
#include <cstring>

using namespace std;

int main(int argc, char *arg[]) {
    if(argc > 1){
        if(strcmp(arg[1],"send") == 0){
          try{
            UDPSocket s = UDPSocket("localhost", "2001", 
                                  UDPSocket::SocketType::client);
            s.send(arg[2]);
          } catch(const char *msg) {
            cout << "Error message: " << msg << endl;
          }
        } else {
            string str;
            UDPSocket s = UDPSocket("localhost", "2001", 
                                  UDPSocket::SocketType::server);
            while(1){
              s.receive(str, 100);
              cout << str << endl;
            }
        }

    } else {
        cerr << "Need at least one argument receive or send" << endl;
    }
    return 0;
}
