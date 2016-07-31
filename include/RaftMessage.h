#ifndef RaftMessage_h
#define RaftMessage_h

#include <rapidjson/document.h>
#include <string>
#include <MessageContent.h>


class RaftMessage {
    public:
        std::string to_string() const;
        virtual void parse_json(std::string json) const {};
};

#endif