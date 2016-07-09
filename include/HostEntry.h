#ifndef HostEntry_h
#define HostEntry_h

#include <string>

using namespace std;

class HostEntry{
  public:
    HostEntry(int no, string host, string service);
    inline int getNo() { return m_no;};
    inline string getHost() { return m_host;};
    inline string getService() {return m_service;};
  private:
    int m_no;
    string m_host;
    string m_service;
};

inline HostEntry::HostEntry(int no, string host, string service)
  : m_no(no), m_host(host), m_service(service) {}

#endif
