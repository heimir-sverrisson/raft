#ifndef RequestVote_h
#define RequestVote_h

#include <rapidjson/document.h>
#include <string>

class RequestVote {
  public:
    RequestVote();
    RequestVote(const RequestVote&);
    RequestVote(int term, int candidateId, int lastLogIndex, int lastLogTerm);
    void addEntry(rapidjson::Value& v);
    void parse_json(std::string json);
    std::string to_string();
    std::string get_entries();
    inline int getTerm(){ return m_term; };
    inline int getCandidateId(){ return m_candidateId; };
    inline int getLastLogIndex(){ return m_lastLogIndex; };
    inline int getLastLogTerm(){ return m_lastLogTerm; };
  private:
    void addInt(rapidjson::Value& o, const char *key, int value);
    rapidjson::Document m_d;
    rapidjson::Value m_o;
    rapidjson::Document::AllocatorType& m_allocator;
    int m_term;
    int m_candidateId;
    int m_lastLogIndex;
    int m_lastLogTerm;
};

#endif
