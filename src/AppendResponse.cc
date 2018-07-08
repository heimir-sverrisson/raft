#include <AppendResponse.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <boost/log/trivial.hpp>

using namespace rapidjson;

AppendResponse::AppendResponse() : allocator_(d_.GetAllocator()) {
    nodeId_ = 0;
    term_ = 0;
    success_ = 0;
    o_ = Value(kObjectType);

    addInt(o_, "nodeId", nodeId_);
    addInt(o_, "term", term_);
    addInt(o_, "success", success_);
}

AppendResponse::AppendResponse(const AppendResponse& cpy)
    : allocator_(d_.GetAllocator()) {
    nodeId_ = cpy.nodeId_;
    term_ = cpy.term_;
    success_ = cpy.success_;
    d_.CopyFrom(cpy.d_, allocator_);
    o_.CopyFrom(cpy.o_, allocator_);
}

AppendResponse::AppendResponse(int nodeId, int term, int success)
    : allocator_(d_.GetAllocator()) {
    nodeId_ = nodeId;
    term_ = term;
    success_ = success;
    o_ = Value(kObjectType);

    addInt(o_, "nodeId", nodeId);
    addInt(o_, "term", term);
    addInt(o_, "success", success);
}

void AppendResponse::addInt(Value& o, const char* key, int value) {
    Value k(key, allocator_);
    Value v(value);
    o.AddMember(k, v, allocator_);
}

std::string AppendResponse::to_string() const {
    std::string str;
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    o_.Accept(writer);
    str = buffer.GetString();
    return str;
}

void AppendResponse::parse_json(std::string json) {
    Document d;
    ParseResult result = d.Parse(json.c_str());
    if (!result) {
        BOOST_LOG_TRIVIAL(error) << json << ": "
                                 << GetParseError_En(result.Code()) << " at "
                                 << result.Offset();
        return;
    }
    // BOOST_LOG_TRIVIAL(info) << "AppendResponse: " << json;
    o_ = Value(kObjectType);
    Value& v_nodeId = d["nodeId"];
    nodeId_ = v_nodeId.GetInt();
    o_.AddMember("nodeId", v_nodeId, allocator_);

    Value& v_term = d["term"];
    term_ = v_term.GetInt();
    o_.AddMember("term", v_term, allocator_);

    Value& v_success = d["success"];
    success_ = v_success.GetInt();
    o_.AddMember("success", v_success, allocator_);
}
