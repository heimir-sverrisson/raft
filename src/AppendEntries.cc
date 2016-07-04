#include <AppendEntries.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <boost/log/trivial.hpp>

#include <iostream>

using namespace rapidjson;

AppendEntries::AppendEntries()
  : m_allocator(m_d.GetAllocator())
{
  m_term = 0;
  m_leaderId = 0;
  m_prevLogIndex = 0;
  m_prevLogTerm = 0;
  m_leaderCommit = 0;
  BOOST_LOG_TRIVIAL(info) << "AppendEntries created no params";
}

AppendEntries::AppendEntries(const AppendEntries& cpy)
  : m_allocator(m_d.GetAllocator())
{
  m_term = cpy.m_term;
  m_leaderId = cpy.m_leaderId;
  m_prevLogIndex = cpy.m_prevLogIndex;
  m_prevLogTerm = cpy.m_prevLogTerm;
  m_leaderCommit = m_leaderCommit;
  m_d.CopyFrom(cpy.m_d, m_allocator);
  m_o.CopyFrom(cpy.m_o, m_allocator);
}

AppendEntries::AppendEntries(int term, int leaderId, int prevLogIndex, int prevLogTerm, int leaderCommit)
  : m_allocator(m_d.GetAllocator())
{
  m_term = term;
  m_leaderId = leaderId;
  m_prevLogIndex = prevLogIndex;
  m_prevLogTerm = prevLogTerm;
  m_leaderCommit = leaderCommit;
  m_o = Value(kObjectType);

  addInt(m_o, "term", term);
  addInt(m_o, "leaderId", leaderId);
  addInt(m_o, "prevLogIndex", prevLogIndex);
  addInt(m_o, "prevLogTerm", prevLogTerm);
  addInt(m_o, "leaderCommit", leaderCommit);
  Value k("entries", m_allocator);
  m_o.AddMember(k, Value(kArrayType), m_allocator);
  BOOST_LOG_TRIVIAL(info) << "AppendEntries created w/ params";
}

void
AppendEntries::addInt(Value& o, const char *key, int value){
  Value k(key, m_allocator);
  Value v(value);
  o.AddMember(k, v, m_allocator);
}

std::string
AppendEntries::to_string(){
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  m_o.Accept(writer);
  str = buffer.GetString();
  return str;
}

std::string
AppendEntries::get_entries(){
  std::string str;
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  m_o["entries"].Accept(writer);
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
  m_o = Value(kObjectType);
  Value& v_term = d["term"];
  m_term = v_term.GetInt();
  m_o.AddMember("term",v_term, m_allocator);

  Value& v_leaderId = d["leaderId"];
  m_leaderId = v_leaderId.GetInt();
  m_o.AddMember("leaderId",v_leaderId, m_allocator);

  Value& v_prevLogIndex = d["prevLogIndex"];
  m_prevLogIndex = v_prevLogIndex.GetInt();
  m_o.AddMember("prevLogIndex",v_prevLogIndex, m_allocator);

  Value& v_prevLogTerm = d["prevLogTerm"];
  m_prevLogTerm = v_prevLogTerm.GetInt();
  m_o.AddMember("prevLogTerm",v_prevLogTerm, m_allocator);

  Value& v_leaderCommit = d["leaderCommit"];
  m_leaderCommit = v_leaderCommit.GetInt();
  m_o.AddMember("leaderCommit",v_leaderCommit, m_allocator);

  Value v_entries;
  v_entries.CopyFrom(d["entries"], m_allocator);
  m_o.AddMember("entries", v_entries, m_allocator);
}

void
AppendEntries::addEntry(Value& v){
 m_o["entries"].PushBack(v, m_allocator);
}
