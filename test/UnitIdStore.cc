#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AppendEntriesTest
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <IdStore.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( id_store )


BOOST_AUTO_TEST_CASE( set_and_get )
{
  log_init();
  IdStore s;
  string id = "id-1";
  s.createId(id);
  BOOST_REQUIRE( s.getValue(id, 1944) == 0 );
  BOOST_REQUIRE( s.getValue(id, 1944) == 1944 );
  // Non-existing Id throws an exception
  try{
    s.getValue("non-existing id", 100);
  } catch(string str) {
    BOOST_REQUIRE(str.find("Nonexistent id: ") == 0);
  }
  // Zero is not an allowed range
  try{
    s.getValue(id, 0);
  } catch(string str) {
    BOOST_REQUIRE(str.find("The range value must be positive!") == 0);
  }
}

BOOST_AUTO_TEST_SUITE_END()
