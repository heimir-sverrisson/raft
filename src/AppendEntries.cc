#include <AppendEntries.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <boost/log/trivial.hpp>

#include <iostream>

using namespace rapidjson;

AppendEntries::AppendEntries()
  : allocator_(d_.GetAllocator())
{
  term_ = 0;
  leaderId_ = 0;
  prevLogIndex_ = 0;
  prevLogTerm_ = 0;
  leaderCommit_ = 0;
  o_ = Value(kObjectType);

  addInt(o_, "term", term_);
  addInt(o_, "leaderId", leaderId_);
  addInt(o_, "prevLogIndex", prevLogIndex_);
  addInt(o_, "prevLogTerm", prevLogTerm_);
  addInt(o_, "leaderCommit", leaderCommit_);
  o_.AddMember("entries", Value(kArrayType), allocator_);
}

AppendEntries::AppendEntries(const AppendEntries& cpy)
  : allocator_(d_.GetAllocator())
{
  term_ = cpy.term_;
  leaderId_ = cpy.leaderId_;
  prevLogIndex_ = cpy.prevLogIndex_;
  prevLogTerm_ = cpy.prevLogTerm_;
  leaderCommit_ = cpy.leaderCommit_;
  d_.CopyFrom(cpy.d_, allocator_);
  o_.CopyFrom(cpy.o_, allocator_);
}

AppendEntries::AppendEntries(int term, int leaderId, int prevLogIndex, int prevLogTerm, int leaderCommit)
  : allocator_(d_.GetAllocator())
{
  term_ = term;
  leaderId_ = leaderId;
  prevLogIndex_ = prevLogIndex;
  prevLogTerm_ = prevLogTerm;
  leaderCommit_ = leaderCommit;
  o_ = Value(kObjectType);

  addInt(o_, "term", term);
  addInt(o_, "leaderId", leaderId);
  addInt(o_, "prevLogIndex", prevLogIndex);
  addInt(o_, "prevLogTerm", prevLogTerm);
  addInt(o_, "leaderCommit", leaderCommit);
  o_.AddMember("entries", Value(kArrayType), allocator_);
}

void
AppendEntries::addInt(Value& o, const char *key, int value){
  Value k(key, allocator_);
  Value v(value);
  o.AddMember(k, v, allocator_);
}

std::string
AppendEntries::to_string() const{
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  o_.Accept(writer);
  str = buffer.GetString();
  return str;
}

std::string
AppendEntries::get_entries() const{
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  o_["entries"].Accept(writer);
  str = buffer.GetString();
  return str;
}

void
AppendEntries::parse_json(std::string json){
  Document d;
  ParseResult result = d.Parse(json.c_str());
  if(!result){
    BOOST_LOG_TRIVIAL(error) << json << ": " <<
      GetParseError_En(result.Code()) << " at " << result.Offset();
    return;
  }
  o_ = Value(kObjectType);
  Value& v_term = d["term"];
  term_ = v_term.GetInt();
  o_.AddMember("term",v_term, allocator_);

  Value& v_leaderId = d["leaderId"];
  leaderId_ = v_leaderId.GetInt();
  o_.AddMember("leaderId",v_leaderId, allocator_);

  Value& v_prevLogIndex = d["prevLogIndex"];
  prevLogIndex_ = v_prevLogIndex.GetInt();
  o_.AddMember("prevLogIndex",v_prevLogIndex, allocator_);

  Value& v_prevLogTerm = d["prevLogTerm"];
  prevLogTerm_ = v_prevLogTerm.GetInt();
  o_.AddMember("prevLogTerm",v_prevLogTerm, allocator_);

  Value& v_leaderCommit = d["leaderCommit"];
  leaderCommit_ = v_leaderCommit.GetInt();
  o_.AddMember("leaderCommit",v_leaderCommit, allocator_);

  Value v_entries;
  v_entries.CopyFrom(d["entries"], allocator_);
  o_.AddMember("entries", v_entries, allocator_);
}

void
AppendEntries::addEntry(Value& v){
 o_["entries"].PushBack(v, allocator_);
}
