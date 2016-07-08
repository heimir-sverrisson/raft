#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AppendEntriesTest
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <KeyValueStore.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( KeyValue )


BOOST_AUTO_TEST_CASE( set_and_get )
{
  log_init();
  KeyValueStore s;
  string key = "key";
  string value = "value";
  s.setValue(key, value);
  BOOST_REQUIRE( s.getValue(key) == value );
  value = "new value";
  s.setValue(key, value);
  BOOST_REQUIRE( s.getValue(key) == value );
  // Non-existing keys return an empty string
  BOOST_REQUIRE( s.getValue("non-existing key") == "" );
}

BOOST_AUTO_TEST_SUITE_END()
