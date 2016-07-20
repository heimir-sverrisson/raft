#include <HostList.h>
#include <boost/log/trivial.hpp>
#include <fstream>
#include <sstream>

HostList::HostList(string filename, int myId){
  ifstream infile(filename);
  if(!infile){
    string str = "File: " + filename + " not found or access denied!";
    BOOST_LOG_TRIVIAL(error) << str;
    throw(str);
  }
  vector<int> hostIndices;
  BOOST_LOG_TRIVIAL(info) << "Start of host list";
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
    BOOST_LOG_TRIVIAL(info) << h.to_string();
    m_hosts.push_back(h);
  }
  BOOST_LOG_TRIVIAL(info) << "End of host list";
  // Check if myId is an index in the file
  if(find(hostIndices.begin(), hostIndices.end(), myId) == hostIndices.end()){
    string str = "Index: " + to_string(myId) + " not found in file: " + filename;
    BOOST_LOG_TRIVIAL(error) << str;
    throw(str);
  }
}

HostEntry 
HostList::getHostById(int id){
  int theId = -1;
  HostEntry he(0,"","");
  for(auto& h : m_hosts){
    if(h.getNodeId() == id){
      theId = h.getNodeId();
      he = h;
      break;
    }
  }
  if(theId == -1){ // Not found
    string str = "Id: " + to_string(id) + ", not found in HostList";
    BOOST_LOG_TRIVIAL(error) << str;
    throw(str);
  }
  return he;
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
