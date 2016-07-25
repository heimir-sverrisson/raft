#ifndef HostList_h
#define HostList_h

#include <string>
#include <vector>
#include <HostEntry.h>

using namespace std;

class HostList{
  public:
    HostList(string filename, int myId);
    inline vector<HostEntry> getHosts() { return hosts_; };
    HostEntry getHostById(int id);
  private:
    vector<HostEntry> hosts_;
    string trim(string& str);
};

#endif
