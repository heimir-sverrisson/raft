#include <HostList.h>
#include <boost/log/trivial.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

HostList::HostList(string filename, int myId)
  : m_myId(myId) {
  ifstream infile(filename);
  if(!infile){
    string str = "File: " + filename + " not found or access denied!";
    BOOST_LOG_TRIVIAL(error) << str;
    throw(str);
  }
  vector<int> hostIndices;
  while(infile){
    string s;
    if(!getline(infile, s)){
      break;
    }
    istringstream ss(s);
    vector<string> rec;
    while(ss){
      string s;
      if(!getline(ss, s, ',')){
        break;
      }
      rec.push_back(trim(s));
    }
    int hostIndex = stoi(rec[0]); // Will throw an exception on malformed data
    hostIndices.push_back(hostIndex);
    HostEntry h(hostIndex, rec[1], rec[2]);
    cout << h.to_string() << endl;
    m_hosts.push_back(h);
  }
  // Check if myId is an index in the file
  if(find(hostIndices.begin(), hostIndices.end(), myId) == hostIndices.end()){
    string str = "Index: " + to_string(myId) + " not found in file: " + filename;
    BOOST_LOG_TRIVIAL(error) << str;
    throw(str);
  }
}

string
HostList::trim(string& str){
  string spaces = " \t";
  size_t first = str.find_first_not_of(spaces);
  size_t last  = str.find_last_not_of(spaces);
  if(first == string::npos && last == string::npos)
    return ""; // in case of empty string or just containg spaces
  return str.substr(first, (last-first+1));
}
