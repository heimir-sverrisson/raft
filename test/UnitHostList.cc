#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HostList
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <HostList.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( host_list )


BOOST_AUTO_TEST_CASE( construct_from_file )
{
  log_init();
  HostList hl("test/hostlist.txt", 2);
  BOOST_REQUIRE( 0 == 0 );
  // Non-existing index throws an exception
  try{
    HostList ng("test/hostlist.txt", 100);
  } catch(string str) {
    BOOST_REQUIRE(str.find("Index: 100") == 0);
  }
  // Non-existing file throws an exception
  try{
    HostList ng("test/nonexistingfile.txt", 100);
  } catch(string str) {
    BOOST_REQUIRE(str.find("File: test/nonexisting") == 0);
  }
}

BOOST_AUTO_TEST_SUITE_END()
