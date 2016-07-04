#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AppendEntriesTest
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <AppendEntries.h>

BOOST_AUTO_TEST_SUITE( raft )


BOOST_AUTO_TEST_CASE( default_constructor )
{
  log_init();
  AppendEntries ae;
  rapidjson::Value v1(1);
  rapidjson::Value v2(2);
  ae.addEntry(v1);
  ae.addEntry(v2);
  BOOST_REQUIRE( ae.get_term() == 0 );
  BOOST_REQUIRE( ae.get_leaderId() == 0 );
  BOOST_REQUIRE( ae.get_prevLogIndex() == 0 );
  BOOST_REQUIRE( ae.get_prevLogTerm() == 0 );
  BOOST_REQUIRE( ae.get_leaderCommit() == 0 );
  BOOST_REQUIRE( ae.get_entries().compare("[1,2]") == 0 );
}

BOOST_AUTO_TEST_CASE( constructor )
{
  AppendEntries ae(1, 2, 2, 3, 4);
  rapidjson::Value v1(1);
  rapidjson::Value v2(2);
  ae.addEntry(v2);
  ae.addEntry(v1);
  BOOST_REQUIRE( ae.get_term() == 1 );
  BOOST_REQUIRE( ae.get_leaderId() == 2 );
  BOOST_REQUIRE( ae.get_prevLogIndex() == 2 );
  BOOST_REQUIRE( ae.get_prevLogTerm() == 3 );
  BOOST_REQUIRE( ae.get_leaderCommit() == 4 );
  BOOST_REQUIRE( ae.get_entries().compare("[2,1]") == 0 );
}

BOOST_AUTO_TEST_CASE( copy_constructor )
{
  AppendEntries aeo(1, 2, 2, 3, 4);
  rapidjson::Value v1(1);
  rapidjson::Value v2(2);
  aeo.addEntry(v2);
  aeo.addEntry(v1);
  AppendEntries ae = aeo;
  BOOST_REQUIRE( ae.get_term() == 1 );
  BOOST_REQUIRE( ae.get_leaderId() == 2 );
  BOOST_REQUIRE( ae.get_prevLogIndex() == 2 );
  BOOST_REQUIRE( ae.get_prevLogTerm() == 3 );
  BOOST_REQUIRE( ae.get_leaderCommit() == 4 );
  BOOST_REQUIRE( ae.get_entries().compare("[2,1]") == 0 );
}

BOOST_AUTO_TEST_CASE( parsing )
{
  std::string json ="{\"term\":5,\"leaderId\":6,\"prevLogIndex\":7,\"prevLogTerm\":8,\
\"leaderCommit\":9,\"entries\":[14,13,12,11,10,9,8,7,6,5]}";
  AppendEntries ae;
  ae.parse_json(json);
  BOOST_REQUIRE( ae.get_term() == 5 );
  BOOST_REQUIRE( ae.get_leaderId() == 6 );
  BOOST_REQUIRE( ae.get_prevLogIndex() == 7 );
  BOOST_REQUIRE( ae.get_prevLogTerm() == 8 );
  BOOST_REQUIRE( ae.get_leaderCommit() == 9 );
  BOOST_REQUIRE( ae.get_entries().compare("[14,13,12,11,10,9,8,7,6,5]") == 0 );
  BOOST_REQUIRE( ae.to_string().compare(json) == 0 );
}

BOOST_AUTO_TEST_SUITE_END()
