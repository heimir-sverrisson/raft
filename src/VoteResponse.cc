#include <VoteResponse.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <boost/log/trivial.hpp>

using namespace rapidjson;

VoteResponse::VoteResponse()
  : allocator_(d_.GetAllocator())
{
  nodeId_ = 0;
  term_ = 0;
  voteGranted_ = 0;
  o_ = Value(kObjectType);

  addInt(o_, "nodeId", nodeId_);
  addInt(o_, "term", term_);
  addInt(o_, "voteGranted", voteGranted_);
}

VoteResponse::VoteResponse(const VoteResponse& cpy)
  : allocator_(d_.GetAllocator())
{
  nodeId_ = cpy.nodeId_;
  term_ = cpy.term_;
  voteGranted_ = cpy.voteGranted_;
  d_.CopyFrom(cpy.d_, allocator_);
  o_.CopyFrom(cpy.o_, allocator_);
}

VoteResponse::VoteResponse(int nodeId, int term, int voteGranted)
  : allocator_(d_.GetAllocator())
{
  nodeId_ = nodeId;
  term_ = term;
  voteGranted_ = voteGranted;
  o_ = Value(kObjectType);

  addInt(o_, "nodeId", nodeId);
  addInt(o_, "term", term);
  addInt(o_, "voteGranted", voteGranted);
}

void
VoteResponse::addInt(Value& o, const char *key, int value){
  Value k(key, allocator_);
  Value v(value);
  o.AddMember(k, v, allocator_);
}

std::string
VoteResponse::to_string() const {
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  o_.Accept(writer);
  str = buffer.GetString();
  return str;
}

void
VoteResponse::parse_json(std::string json){
  Document d;
  ParseResult result = d.Parse(json.c_str());
  if(!result){
    BOOST_LOG_TRIVIAL(error) << json << ": " <<
      GetParseError_En(result.Code()) << " at " << result.Offset();
    return;
  }
  BOOST_LOG_TRIVIAL(info) << "VoteResponse: " << json;
  o_ = Value(kObjectType);
  Value& v_nodeId = d["nodeId"];
  nodeId_ = v_nodeId.GetInt();
  o_.AddMember("nodeId",v_nodeId, allocator_);

  Value& v_term = d["term"];
  term_ = v_term.GetInt();
  o_.AddMember("term",v_term, allocator_);

  Value& v_voteGranted = d["voteGranted"];
  voteGranted_ = v_voteGranted.GetInt();
  o_.AddMember("voteGranted",v_voteGranted, allocator_);
}
