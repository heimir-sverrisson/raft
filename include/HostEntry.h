#ifndef HostEntry_h
#define HostEntry_h

#include <string>

using namespace std;

class HostEntry{
  public:
    HostEntry(int nodeId, string host, string service);
    inline int getNodeId() { return m_nodeId;};
    inline string getHost() { return m_host;};
    inline string getService() {return m_service;};
    inline string to_string() { 
      return std::to_string(m_nodeId) + ": \"" + m_host + "\", \"" + m_service + "\"";
    };
  private:
    int m_nodeId;
    string m_host;
    string m_service;
};

inline HostEntry::HostEntry(int nodeId, string host, string service)
  : m_nodeId(nodeId), m_host(host), m_service(service) {}

#endif
