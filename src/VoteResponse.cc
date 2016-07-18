#include <VoteResponse.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <boost/log/trivial.hpp>

using namespace rapidjson;

VoteResponse::VoteResponse()
  : m_allocator(m_d.GetAllocator())
{
  m_nodeId = 0;
  m_term = 0;
  m_voteGranted = 0;
  m_o = Value(kObjectType);

  addInt(m_o, "nodeId", m_nodeId);
  addInt(m_o, "term", m_term);
  addInt(m_o, "voteGranted", m_voteGranted);
}

VoteResponse::VoteResponse(const VoteResponse& cpy)
  : m_allocator(m_d.GetAllocator())
{
  m_nodeId = cpy.m_nodeId;
  m_term = cpy.m_term;
  m_voteGranted = cpy.m_voteGranted;
  m_d.CopyFrom(cpy.m_d, m_allocator);
  m_o.CopyFrom(cpy.m_o, m_allocator);
}

VoteResponse::VoteResponse(int nodeId, int term, int voteGranted)
  : m_allocator(m_d.GetAllocator())
{
  m_nodeId = nodeId;
  m_term = term;
  m_voteGranted = voteGranted;
  m_o = Value(kObjectType);

  addInt(m_o, "nodeId", nodeId);
  addInt(m_o, "term", term);
  addInt(m_o, "voteGranted", voteGranted);
}

void
VoteResponse::addInt(Value& o, const char *key, int value){
  Value k(key, m_allocator);
  Value v(value);
  o.AddMember(k, v, m_allocator);
}

std::string
VoteResponse::to_string(){
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  m_o.Accept(writer);
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
  m_o = Value(kObjectType);
  Value& v_nodeId = d["nodeId"];
  m_nodeId = v_nodeId.GetInt();
  m_o.AddMember("nodeId",v_nodeId, m_allocator);

  Value& v_term = d["term"];
  m_term = v_term.GetInt();
  m_o.AddMember("term",v_term, m_allocator);

  Value& v_voteGranted = d["voteGranted"];
  m_voteGranted = v_voteGranted.GetInt();
  m_o.AddMember("voteGranted",v_voteGranted, m_allocator);
}
