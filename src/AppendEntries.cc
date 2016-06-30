#include <AppendEntries.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

AppendEntries::AppendEntries() {
  m_term = 0;
  m_leaderId = 0;
  m_prevLogIndex = 0;
  m_prevLogTerm = 0;
  m_leaderCommit = 0;
}

AppendEntries::AppendEntries(int term, int leaderId, int prevLogIndex, int prevLogTerm, int leaderCommit) {
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
  Value k("entries", m_d.GetAllocator());
  m_o.AddMember(k, Value(kArrayType), m_d.GetAllocator());
}

void
AppendEntries::addInt(Value& o, const char *key, int value){
  Value k(key, m_d.GetAllocator());
  Value v(value);
  o.AddMember(k, v, m_d.GetAllocator());
}

const char *
AppendEntries::to_string(){
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  m_o.Accept(writer);
  return buffer.GetString();
}

const char *
AppendEntries::get_entries(){
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  m_o["entries"].Accept(writer);
  return buffer.GetString();
}

void
AppendEntries::parse_json(const char* json){
  Document d;
  ParseResult result = d.Parse(json);
  if(!result){
    return;
  }
  Value& v_term = d["term"];
  m_term = v_term.GetInt();
  Value& v_leaderId = d["leaderId"];
  Value& v_entries = d["entries"];
  m_o = Value(kObjectType);
  Value k("entries", m_d.GetAllocator());
  m_o.AddMember(k, v_entries, m_d.GetAllocator());
  m_leaderId = v_leaderId.GetInt();
}

void
AppendEntries::addEntry(Value& v){
 m_o["entries"].PushBack(v, m_d.GetAllocator());
}
