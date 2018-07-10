#include <RaftProtocol.h>

// This code runs the finate state machine

namespace raft_fsm {

void run(Receiver& r, ServerState& ss) {
    RaftProtocol rp(boost::ref(r), boost::ref(ss));
    rp.start();
    while (true) {
        int timeout = rp.getTimeout();
        timeout = (timeout > 1000) ? 500 : timeout;
        BOOST_LOG_TRIVIAL(info) << "Timeout is: " << timeout;
        WakeupType ret = r.waitForMessage(timeout);
        switch (ret) {
            case notRunning:
                BOOST_LOG_TRIVIAL(error) << "Receiver is not running!";
                return;
            case timedOut: {
                BOOST_LOG_TRIVIAL(info) << "Timed out";
                rp.process_event(Timeout());
            } break;
            case gotMessage:
                // Process the message
                MessageContent m = r.getMessage();
                switch (m.messageType) {
                    case appendEntries: {
                        AppendEntries ae;
                        ae.parse_json(m.json);
                        BOOST_LOG_TRIVIAL(info) << "Got AppendEntries";
                        rp.process_event(GotAppendEntries(ae));
                    } break;
                    case requestVote: {
                        RequestVote rv;
                        rv.parse_json(m.json);
                        BOOST_LOG_TRIVIAL(info) << "Got RequestVote";
                        rp.process_event(GotRequestVote(rv));
                    } break;
                    case voteResponse: {
                        VoteResponse vr;
                        vr.parse_json(m.json);
                        BOOST_LOG_TRIVIAL(info) << "Got VoteResponse";
                        rp.process_event(GotVoteResponse(vr));
                    } break;
                    case appendResponse: {
                        AppendResponse ar;
                        ar.parse_json(m.json);
                        BOOST_LOG_TRIVIAL(info) << "Got AppendResponse";
                        rp.process_event(GotAppendResponse(ar));
                    } break;
                    case client:
                    default:
                        BOOST_LOG_TRIVIAL(error)
                            << "Unknown MessageType: " << m.messageType;
                }
                break;
        }
    }
}
}  // namespace
