#ifndef IdStore_h
#define IdStore_h

#include <string>
#include <cstdint>
#include <unordered_map>

using namespace std;

class IdStore {
public:
  void dump();
  inline void createId(string id) { m_[id] = 0; };
  uint64_t getValue(string id, uint64_t range); // Return start of a range
private:
  unordered_map<string, uint64_t> m_;
};
#endif
