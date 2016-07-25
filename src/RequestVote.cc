#include <RequestVote.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <boost/log/trivial.hpp>

using namespace rapidjson;

RequestVote::RequestVote()
  : allocator_(d_.GetAllocator())
{
  term_ = 0;
  candidateId_ = 0;
  lastLogIndex_ = 0;
  lastLogTerm_ = 0;
  o_ = Value(kObjectType);

  addInt(o_, "term", term_);
  addInt(o_, "candidateId", candidateId_);
  addInt(o_, "lastLogIndex", lastLogIndex_);
  addInt(o_, "lastLogTerm", lastLogTerm_);
}

RequestVote::RequestVote(const RequestVote& cpy)
  : allocator_(d_.GetAllocator())
{
  term_ = cpy.term_;
  candidateId_ = cpy.candidateId_;
  lastLogIndex_ = cpy.lastLogIndex_;
  lastLogTerm_ = cpy.lastLogTerm_;
  d_.CopyFrom(cpy.d_, allocator_);
  o_.CopyFrom(cpy.o_, allocator_);
}

RequestVote::RequestVote(int term, int candidateId, int lastLogIndex, int lastLogTerm)
  : allocator_(d_.GetAllocator())
{
  term_ = term;
  candidateId_ = candidateId;
  lastLogIndex_ = lastLogIndex;
  lastLogTerm_ = lastLogTerm;
  o_ = Value(kObjectType);

  addInt(o_, "term", term);
  addInt(o_, "candidateId", candidateId);
  addInt(o_, "lastLogIndex", lastLogIndex);
  addInt(o_, "lastLogTerm", lastLogTerm);
}

void
RequestVote::addInt(Value& o, const char *key, int value){
  Value k(key, allocator_);
  Value v(value);
  o.AddMember(k, v, allocator_);
}

std::string
RequestVote::to_string(){
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  o_.Accept(writer);
  str = buffer.GetString();
  return str;
}

void
RequestVote::parse_json(std::string json){
  Document d;
  ParseResult result = d.Parse(json.c_str());
  if(!result){
    BOOST_LOG_TRIVIAL(error) << json << ": " <<
      GetParseError_En(result.Code()) << " at " << result.Offset();
    return;
  }
  BOOST_LOG_TRIVIAL(info) << "RequestVote: " << json;
  o_ = Value(kObjectType);
  Value& v_term = d["term"];
  term_ = v_term.GetInt();
  o_.AddMember("term",v_term, allocator_);

  Value& v_candidateId = d["candidateId"];
  candidateId_ = v_candidateId.GetInt();
  o_.AddMember("candidateId",v_candidateId, allocator_);

  Value& v_lastLogIndex = d["lastLogIndex"];
  lastLogIndex_ = v_lastLogIndex.GetInt();
  o_.AddMember("lastLogIndex",v_lastLogIndex, allocator_);

  Value& v_lastLogTerm = d["lastLogTerm"];
  lastLogTerm_ = v_lastLogTerm.GetInt();
  o_.AddMember("lastLogTerm",v_lastLogTerm, allocator_);
}
