#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AppendEntriesTest
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <AppendEntries.h>

BOOST_AUTO_TEST_SUITE( raft )


BOOST_AUTO_TEST_CASE( ae_test )
{
  log_init();
  AppendEntries ae;
  BOOST_REQUIRE( ae.get_term() == 0 );
}

BOOST_AUTO_TEST_SUITE_END()
