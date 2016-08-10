#ifndef MessageContent_h
#define MessageContent_h

#include <string>

enum MessageType {
  unknown, client, appendEntries, requestVote, voteResponse, appendResponse
};

struct MessageContent{
    MessageType messageType;
    std::string json;
};
#endif