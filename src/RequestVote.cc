#include <RequestVote.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <boost/log/trivial.hpp>

using namespace rapidjson;

RequestVote::RequestVote()
  : m_allocator(m_d.GetAllocator())
{
  m_term = 0;
  m_candidateId = 0;
  m_lastLogIndex = 0;
  m_lastLogTerm = 0;
  m_o = Value(kObjectType);

  addInt(m_o, "term", m_term);
  addInt(m_o, "candidateId", m_candidateId);
  addInt(m_o, "lastLogIndex", m_lastLogIndex);
  addInt(m_o, "lastLogTerm", m_lastLogTerm);
}

RequestVote::RequestVote(const RequestVote& cpy)
  : m_allocator(m_d.GetAllocator())
{
  m_term = cpy.m_term;
  m_candidateId = cpy.m_candidateId;
  m_lastLogIndex = cpy.m_lastLogIndex;
  m_lastLogTerm = cpy.m_lastLogTerm;
  m_d.CopyFrom(cpy.m_d, m_allocator);
  m_o.CopyFrom(cpy.m_o, m_allocator);
}

RequestVote::RequestVote(int term, int candidateId, int lastLogIndex, int lastLogTerm)
  : m_allocator(m_d.GetAllocator())
{
  m_term = term;
  m_candidateId = candidateId;
  m_lastLogIndex = lastLogIndex;
  m_lastLogTerm = lastLogTerm;
  m_o = Value(kObjectType);

  addInt(m_o, "term", term);
  addInt(m_o, "candidateId", candidateId);
  addInt(m_o, "lastLogIndex", lastLogIndex);
  addInt(m_o, "lastLogTerm", lastLogTerm);
}

void
RequestVote::addInt(Value& o, const char *key, int value){
  Value k(key, m_allocator);
  Value v(value);
  o.AddMember(k, v, m_allocator);
}

std::string
RequestVote::to_string(){
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  m_o.Accept(writer);
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
  m_o = Value(kObjectType);
  Value& v_term = d["term"];
  m_term = v_term.GetInt();
  m_o.AddMember("term",v_term, m_allocator);

  Value& v_candidateId = d["candidateId"];
  m_candidateId = v_candidateId.GetInt();
  m_o.AddMember("candidateId",v_candidateId, m_allocator);

  Value& v_lastLogIndex = d["lastLogIndex"];
  m_lastLogIndex = v_lastLogIndex.GetInt();
  m_o.AddMember("lastLogIndex",v_lastLogIndex, m_allocator);

  Value& v_lastLogTerm = d["lastLogTerm"];
  m_lastLogTerm = v_lastLogTerm.GetInt();
  m_o.AddMember("lastLogTerm",v_lastLogTerm, m_allocator);
}
