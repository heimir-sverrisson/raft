#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HostEntryTest
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <HostEntry.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( host_entry )


BOOST_AUTO_TEST_CASE( create_and_get )
{
  log_init();
  int no = 42;
  string hostname = "the.name.of.the.host";
  string service = "the.service";
  HostEntry h(no, hostname, service);
  BOOST_REQUIRE( h.getNo() == no );
  BOOST_REQUIRE( h.getHost() == hostname );
  BOOST_REQUIRE( h.getService() == service );
}

BOOST_AUTO_TEST_SUITE_END()
