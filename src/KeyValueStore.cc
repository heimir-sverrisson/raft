#include <KeyValueStore.h>
#include <iostream>

void
KeyValueStore::dump(){
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
