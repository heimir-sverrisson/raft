#ifndef KeyValueStore_h
#define KeyValueStore_h

#include <string>
#include <unordered_map>

using namespace std;

class KeyValueStore{
  public:
    void dump();
    inline void setValue(string key, string value) { m[key] = value;};
    inline string getValue(string key) { return m[key];};
  private:
    unordered_map<string, string> m; 
};
#endif
