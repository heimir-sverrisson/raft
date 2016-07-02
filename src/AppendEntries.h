#ifndef AppendEntries_h
#define AppendEntries_h

#include <rapidjson/document.h>
#include <string>

class AppendEntries {
  public:
    AppendEntries();
    // AppendEntries(const AppendEntries&);
    AppendEntries(int term, int leaderId, int prevLogIndex, int prevLogTerm, int leaderCommit);
    void addEntry(rapidjson::Value& v);
    void parse_json(std::string json);
    void to_string(std::string& str);
    void get_entries(std::string& str);
    int get_term();
    int get_leaderId();
    int get_prevLogIndex();
    int get_prevLogTerm();
    int get_leaderCommit();
  private:
    void addInt(rapidjson::Value& o, const char *key, int value);
    rapidjson::Document m_d;
    rapidjson::Value m_o;
    int m_term;
    int m_leaderId;
    int m_prevLogIndex;
    int m_prevLogTerm;
    int m_leaderCommit;
};

#endif
