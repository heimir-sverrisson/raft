#include <IdStore.h>
#include <iostream>
#include <boost/log/trivial.hpp>

uint64_t
IdStore::getValue(string id, uint64_t range){
  if(m_.find(id) == m_.end()){
    string str = "Nonexistent id: " + id + ", range: " + to_string(range) + " requested!";
    BOOST_LOG_TRIVIAL(error) << str;
    throw str;
  }
  if(range == 0){
    string str = "The range value must be positive!";
    BOOST_LOG_TRIVIAL(error) << str;
    throw str;
  }
  uint64_t prev_value = m_[id];
  m_[id] = prev_value + range;
  return prev_value;
}

void
IdStore::dump(){
  bool start = true;
  for( auto it = m_.begin(); it != m_.end(); ++it){
    if(start){
      start = false;
      cout << it->first << ": " << it->second;
    } else {
      cout << ", " << it->first << ": " << it->second;
    }
  }
  cout << endl;
}
