#ifndef VoteResponse_h
#define VoteResponse_h

#include <RaftMessage.h>
#include <rapidjson/document.h>
#include <string>

class VoteResponse : public virtual RaftMessage {
  public:
    VoteResponse();
    VoteResponse(const VoteResponse&);
    VoteResponse(int nodeId, int term, int voteGranted);
    void parse_json(std::string json);
    std::string to_string();
    inline int getNodeId(){ return nodeId_; };
    inline int getTerm(){ return term_; };
    inline int getVoteGranted(){ return voteGranted_; };
  private:
    void addInt(rapidjson::Value& o, const char *key, int value);
    rapidjson::Document d_;
    rapidjson::Value o_;
    rapidjson::Document::AllocatorType& allocator_;
    int nodeId_;
    int term_;
    int voteGranted_;
};

#endif
