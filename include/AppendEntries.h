#ifndef AppendEntries_h
#define AppendEntries_h

#include <rapidjson/document.h>
#include <string>

class AppendEntries {
  public:
    AppendEntries();
    AppendEntries(const AppendEntries&);
    AppendEntries(int term, int leaderId, int prevLogIndex, int prevLogTerm, int leaderCommit);
    void addEntry(rapidjson::Value& v);
    void parse_json(std::string json);
    std::string to_string();
    std::string get_entries();
    inline int getTerm(){ return m_term; };
    inline int getLeaderId(){ return m_leaderId; };
    inline int getPrevLogIndex(){ return m_prevLogIndex; };
    inline int getPrevLogTerm(){ return m_prevLogTerm; };
    inline int getLeaderCommit(){ return m_leaderCommit; };
  private:
    void addInt(rapidjson::Value& o, const char *key, int value);
    rapidjson::Document m_d;
    rapidjson::Value m_o;
    rapidjson::Document::AllocatorType& m_allocator;
    int m_term;
    int m_leaderId;
    int m_prevLogIndex;
    int m_prevLogTerm;
    int m_leaderCommit;
};

#endif
