#ifndef AppendEntries_h
#define AppendEntries_h

#include <rapidjson/document.h>

using namespace rapidjson;

class AppendEntries {
  public:
    AppendEntries();
    AppendEntries(int term, int leaderId, int prevLogIndex, int prevLogTerm, int leaderCommit);
    void addEntry(Value& v);
    void parse_json(const char* json);
    const char* to_string();
    const char* get_entries();
    int get_term();
    int get_leaderId();
    int get_prevLogIndex();
    int get_prevLogTerm();
    int get_leaderCommit();
  private:
    void addInt(Value& o, const char *key, int value);
    Document m_d;
    Value m_o;
    int m_term;
    int m_leaderId;
    int m_prevLogIndex;
    int m_prevLogTerm;
    int m_leaderCommit;
};

#endif
