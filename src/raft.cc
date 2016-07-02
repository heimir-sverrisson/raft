#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <UDPSocket.h>
#include <AppendEntries.h>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

// using namespace std;

bool done = false;

void init()
{
    logging::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
    logging::add_file_log
    (
      keywords::file_name = "raft_%N.log",
      keywords::rotation_size = 10 * 1024 * 1024,
      keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%]: %Severity% %ThreadID% %Message%"
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}

void receive_msg(void){
  string str;
  AppendEntries ae;
  BOOST_LOG_TRIVIAL(info) << "Starting receive_msg()";
  UDPSocket s = UDPSocket("localhost", "2001", 
                        UDPSocket::SocketType::server);
  while(!done){
    int ret = s.receive(str, 1000, 1100);
    if(ret < 0){
      BOOST_LOG_TRIVIAL(info) << "Timeout!";
      return;
    }
    if(str.length() == 0){
      BOOST_LOG_TRIVIAL(info) << "Done!";
      return;
    }
    ae.parse_json(str);
    string the_json;
    ae.to_string(the_json);
    cout << the_json << endl;
  }
}

void send_msg(AppendEntries *ae){
  BOOST_LOG_TRIVIAL(info) << "Starting send_msg()";
  try{
    UDPSocket sock = UDPSocket("localhost", "2001", 
                          UDPSocket::SocketType::client);
    // for(std::vector<T>::iterator it = v.begin(); it != v.end(); ++it)
    // for(vector<AppendEntries>::iterator it = aes.begin(); it != aes.end(); ++it){
      string s;
      ae->to_string(s);
      sock.send(s);
      this_thread::sleep_for(chrono::milliseconds(1000));
    // }
    done = true;
    sock.send("");
  } catch(const char *msg) {
    cout << "Error message: " << msg << endl;
  }
}

int main(int argc, char *arg[]) {
  init();
  logging::add_common_attributes();

  BOOST_LOG_TRIVIAL(info) << "Starting main()";
  AppendEntries ae(1, 2, 3, 4, 5);
  using namespace rapidjson;
  for(int i=0; i < 10; i++){
    Value v(10 - i);
    ae.addEntry(v);
  }
  // vector<AppendEntries> aes;
  // aes.push_back(ae);

  thread rcv(&receive_msg);
  thread snd(&send_msg, &ae);
  snd.join();
  rcv.join();
}
