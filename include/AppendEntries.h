#ifndef AppendEntries_h
#define AppendEntries_h

#include <RaftMessage.h>
#include <rapidjson/document.h>
#include <string>

class AppendEntries : public virtual RaftMessage {
  public:
    AppendEntries();
    AppendEntries(const AppendEntries&);
    AppendEntries(int term, int leaderId, int prevLogIndex, int prevLogTerm, int leaderCommit);
    void addEntry(rapidjson::Value& v);
    void parse_json(std::string json);
    std::string to_string() const;
    std::string get_entries() const;
    inline int getTerm() const{ return term_; };
    inline int getLeaderId() const{ return leaderId_; };
    inline int getPrevLogIndex() const{ return prevLogIndex_; };
    inline int getPrevLogTerm() const{ return prevLogTerm_; };
    inline int getLeaderCommit() const{ return leaderCommit_; };
  private:
    void addInt(rapidjson::Value& o, const char *key, int value);
    rapidjson::Document d_;
    rapidjson::Value o_;
    rapidjson::Document::AllocatorType& allocator_;
    int term_;
    int leaderId_;
    int prevLogIndex_;
    int prevLogTerm_;
    int leaderCommit_;
};

#endif
