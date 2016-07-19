#ifndef VoteResponse_h
#define VoteResponse_h

#include <rapidjson/document.h>
#include <string>

class VoteResponse {
  public:
    VoteResponse();
    VoteResponse(const VoteResponse&);
    VoteResponse(int nodeId, int term, int voteGranted);
    void parse_json(std::string json);
    std::string to_string();
    inline int getNodeId(){ return m_nodeId; };
    inline int getTerm(){ return m_term; };
    inline int getVoteGranted(){ return m_voteGranted; };
  private:
    void addInt(rapidjson::Value& o, const char *key, int value);
    rapidjson::Document m_d;
    rapidjson::Value m_o;
    rapidjson::Document::AllocatorType& m_allocator;
    int m_nodeId;
    int m_term;
    int m_voteGranted;
};

#endif
