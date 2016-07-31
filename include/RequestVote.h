#ifndef RequestVote_h
#define RequestVote_h

#include <RaftMessage.h>
#include <rapidjson/document.h>
#include <string>

class RequestVote : public virtual RaftMessage {
  public:
    RequestVote();
    RequestVote(const RequestVote&);
    RequestVote(int term, int candidateId, int lastLogIndex, int lastLogTerm);
    void addEntry(rapidjson::Value& v);
    void parse_json(std::string json);
    std::string to_string();
    std::string get_entries();
    inline int getTerm(){ return term_; };
    inline int getCandidateId(){ return candidateId_; };
    inline int getLastLogIndex(){ return lastLogIndex_; };
    inline int getLastLogTerm(){ return lastLogTerm_; };
  private:
    void addInt(rapidjson::Value& o, const char *key, int value);
    rapidjson::Document d_;
    rapidjson::Value o_;
    rapidjson::Document::AllocatorType& allocator_;
    int term_;
    int candidateId_;
    int lastLogIndex_;
    int lastLogTerm_;
};

#endif
