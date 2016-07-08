#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RequestVoteTest
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <RequestVote.h>

BOOST_AUTO_TEST_SUITE( raft )


BOOST_AUTO_TEST_CASE( default_constructor )
{
  log_init();
  RequestVote rv;
  BOOST_REQUIRE( rv.get_term() == 0 );
  BOOST_REQUIRE( rv.get_candidateId() == 0 );
  BOOST_REQUIRE( rv.get_lastLogIndex() == 0 );
  BOOST_REQUIRE( rv.get_lastLogTerm() == 0 );
}

BOOST_AUTO_TEST_CASE( constructor )
{
  RequestVote rv(1, 2, 2, 3);
  BOOST_REQUIRE( rv.get_term() == 1 );
  BOOST_REQUIRE( rv.get_candidateId() == 2 );
  BOOST_REQUIRE( rv.get_lastLogIndex() == 2 );
  BOOST_REQUIRE( rv.get_lastLogTerm() == 3 );
}

BOOST_AUTO_TEST_CASE( copy_constructor )
{
  RequestVote rvo(1, 5, 7, 3);
  RequestVote rv = rvo;
  BOOST_REQUIRE( rv.get_term() == 1 );
  BOOST_REQUIRE( rv.get_candidateId() == 5 );
  BOOST_REQUIRE( rv.get_lastLogIndex() == 7 );
  BOOST_REQUIRE( rv.get_lastLogTerm() == 3 );
}

BOOST_AUTO_TEST_CASE( parsing )
{
  std::string json ="{\"term\":5,\"candidateId\":6,\"lastLogIndex\":7,\"lastLogTerm\":13}";
  RequestVote rv;
  rv.parse_json(json);
  BOOST_REQUIRE( rv.get_term() == 5 );
  BOOST_REQUIRE( rv.get_candidateId() == 6 );
  BOOST_REQUIRE( rv.get_lastLogIndex() == 7 );
  BOOST_REQUIRE( rv.get_lastLogTerm() == 13 );
  BOOST_REQUIRE( rv.to_string().compare(json) == 0 );
}

BOOST_AUTO_TEST_SUITE_END()
