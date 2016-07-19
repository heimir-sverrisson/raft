#include <Raft.h>
#include <raft_util.h>
#include <boost/log/trivial.hpp>
#include <iostream>

int
main(int argc, char *argv[]){
  log_init();
  if(argc < 3){
    cerr << "Usage:" << endl << "\t" << argv[0] << " hostfile hostId" << endl;
    return 1;
  }
  BOOST_LOG_TRIVIAL(info) << "Starting main()";
  int hostId = -1;
  try{
    hostId = stoi(argv[2]);
  } catch(invalid_argument) {
    cerr << argv[2] << " is not a number" << endl;
    return 3;
  }
  try{
    Raft r(argv[1], hostId);
    r.run();
  } catch(string str) {
    cerr << "Runtime exception: " + str << endl;
    return 2;
  } catch(char const *chr) {
    cerr << "Runtime exception: " + string(chr) << endl;
    return 2;
  } catch(char *chr) {
    cerr << "Runtime exception: " + string(chr) << endl;
    return 2;
  }
  return 0;
}
