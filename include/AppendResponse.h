#ifndef AppendResponse_h
#define AppendResponse_h

#include <RaftMessage.h>
#include <rapidjson/document.h>
#include <string>

class AppendResponse : public virtual RaftMessage {
    public:
        AppendResponse();
        AppendResponse(const AppendResponse&);
        AppendResponse(int nodeId, int term, int success);
        void parse_json(std::string json);
        std::string to_string() const;
        inline int getNodeId() const { return nodeId_; };
        inline int getTerm() const { return term_; };
        inline int getSuccess() const { return success_; };
    private:
        void addInt(rapidjson::Value& o, const char *key, int value);
        rapidjson::Document d_;
        rapidjson::Value o_;
        rapidjson::Document::AllocatorType& allocator_;
        int nodeId_;
        int term_;
        int success_; 
};
#endif