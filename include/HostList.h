#ifndef HostList_h
#define HostList_h

#include <string>
#include <vector>
#include <HostEntry.h>

using namespace std;

class HostList{
  public:
    HostList(string filename, int myId);
    vector<HostEntry> getAllHosts();
    HostEntry getHostById(int id);
  private:
    vector<HostEntry> m_hosts;
    string trim(string& str);
};

#endif
