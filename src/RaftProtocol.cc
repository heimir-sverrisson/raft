#include <RaftProtocol.h>

// This code runs the finate state machine

namespace raft_fsm {

    void run(Receiver& r, ServerState& ss){
        RaftProtocol rp(boost::ref(r), boost::ref(ss));
        rp.start();
        while(true){
            BOOST_LOG_TRIVIAL(info) << "Timeout is: " << rp.getTimeout();
            WakeupType ret = r.waitForMessage(rp.getTimeout());
            BOOST_LOG_TRIVIAL(error) << "Return value is: " << ret;
            switch(ret){
                case notRunning:
                    BOOST_LOG_TRIVIAL(error) << "Receiver is not running!";
                    return;
                case timedOut:
                    {
                        BOOST_LOG_TRIVIAL(info) << "Timed out";
                        rp.process_event(Timeout());
                    }
                    break;
                case gotMessage:
                    // Process the message
                    MessageContent m = r.getMessage();
                    switch(m.messageType){
                        case appendEntries:
                        {
                            AppendEntries ae;
                            ae.parse_json(m.json);
                            BOOST_LOG_TRIVIAL(info) << "Got AppendEntries";
                            rp.process_event(GotAppendEntries(ae));
                        }
                        break;
                        case requestVote:
                        {
                            RequestVote rv;
                            rv.parse_json(m.json);
                            BOOST_LOG_TRIVIAL(info) << "Got RequestVote";
                        }
                        break;
                        case voteResponse:
                        {
                            VoteResponse vr;
                            vr.parse_json(m.json);
                            BOOST_LOG_TRIVIAL(info) << "Got VoteResponse";
                        }
                        break;
                        case client:
                        default:
                        BOOST_LOG_TRIVIAL(error) << "Unknown MessageType: " << m.messageType;
                    }
                    break;
            }
        }
    }
} // namespace
