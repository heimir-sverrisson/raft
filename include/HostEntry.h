#ifndef HostEntry_h
#define HostEntry_h

#include <string>

using namespace std;

class HostEntry{
  public:
    HostEntry(int nodeId, string listenAddress, string host, string service);
    inline int getNodeId() { return nodeId_;};
    inline string getListenAddress() { return listenAddress_; };
    inline string getHost() { return host_;};
    inline string getService() {return service_;};
    inline string to_string() { 
      return std::to_string(nodeId_) + ": \"" + listenAddress_ + "\", \"" + host_ + "\", \"" + service_ + "\"";
    };
  private:
    int nodeId_;
    string listenAddress_;
    string host_;
    string service_;
};

inline HostEntry::HostEntry(int nodeId, string listenAddress, string host, string service)
  : nodeId_(nodeId), listenAddress_(listenAddress), host_(host), service_(service) {}

#endif
