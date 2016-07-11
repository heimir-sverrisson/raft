#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AppendEntriesTest
#include <boost/test/unit_test.hpp>
#include <raft_util.h>

#include <UDPSocket.h>

#include <iostream>

BOOST_AUTO_TEST_SUITE( raft )


BOOST_AUTO_TEST_CASE( server_with_timeout )
{
  log_init();
  UDPSocket server("localhost", "2001", SocketType::serverSocket);
  std::string empty;
  int ret = server.receive(empty, 10, 100);
  BOOST_REQUIRE( ret == -1 );
  BOOST_REQUIRE( empty.length() == 0 );
}

// Pretty useless as communications cannot be tested without
// multi threaded code.
BOOST_AUTO_TEST_CASE( client )
{
  log_init();
  std::string str="This string will be sent from client to server";
  UDPSocket client("localhost", "2001", SocketType::clientSocket);
  client.send(str);
  BOOST_REQUIRE( 42 == 42 );
}

BOOST_AUTO_TEST_SUITE_END()
