#ifndef RaftProtocol_h
#define RaftProtocol_h

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/log/trivial.hpp>
#include <Config.h>
#include <Receiver.h>
#include <ServerState.h>
#include <Sender.h>
#include <raft_util.h>

#include <iostream>

namespace msm = boost::msm;
namespace mpl = boost::mpl;

namespace raft_fsm {
    // Events
    struct Timeout {};
    struct HeardServerWithHigherTerm {};
    struct GotMajorityVote {};
    struct GotVoteResponse {};
    struct GotAppendEntries {
        GotAppendEntries(AppendEntries ae) : ae_(ae){};
        AppendEntries ae_;
    };
    struct GotRequestVote {};

    struct raftProtocol_ : public msm::front::state_machine_def<raftProtocol_>
    {
        raftProtocol_(Receiver& r, ServerState& ss) : 
            timeout_(Config::readPeriod), r_(r), ss_(ss){};
        template<class Event, class FSM>
        void on_entry(Event const& evt, FSM& fsm){
            BOOST_LOG_TRIVIAL(info) << "Entering the raftProtocol state machine";
        }

        template<class Event, class FSM>
        void on_exit(Event const& evt, FSM& fsm){
            BOOST_LOG_TRIVIAL(info) << "Exiting the raftProtocol state machine";
        }
      
        // States of the FSM
        struct Follower : public msm::front::state<> 
        {
            template<class Event, class FSM>
            void on_exit(Event const& timeout, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Exiting Follower";
                int tMin = Config::readPeriod/2;
                int t = tMin + rand() % tMin;
                fsm.setTimeout(t);
            }            
        };

        struct Candidate : public msm::front::state<> 
        {
            template<class Event, class FSM>
            void on_entry(Event const& evt, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Entering Candidate";
            }
            template<class Event, class FSM>
            void on_exit(Event const& evt, FSM& fsm){
                fsm.setTimeout(Config::readPeriod);
                BOOST_LOG_TRIVIAL(info) << "Exiting Candidate";
            }
        };
        struct Leader : public msm::front::state<> {
            template<class Event, class FSM>
            void on_entry(Event const& evt, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Entering Leader";
            }

        };

        typedef Follower initial_state;

        // Transition actions
        void startElections(const Timeout& t){
            BOOST_LOG_TRIVIAL(info) << "Starting elections!";
            Sender s;
            s.sendRequestVote(ss_);
        }
        void giveUpLeadership(const GotAppendEntries& evt){
            // Safe to lose this message if I'm not the leader 
            BOOST_LOG_TRIVIAL(info) << "Somebody send me AppendEntries - but I'm the leader";
        }

        void setMyTerm(const HeardServerWithHigherTerm&) {}

        // Guard conditions
        bool isHisTermHigher(const GotAppendEntries& evt){
            int myTerm = ss_.getTerm();
            int hisTerm = evt.ae_.getTerm();
            if (hisTerm > myTerm){
                BOOST_LOG_TRIVIAL(info) << "Lost leadership!";
                return true;
            } else {
                BOOST_LOG_TRIVIAL(info) << "I'm still the leader";
                return false;
            }
        }

        bool votedForMe(const GotVoteResponse& evt) { return true;}

        typedef raftProtocol_ rp; // Shorthand for table
        // Transition Table for protocol
        // DO NOT RENAME THIS VARIABLE !!!
        struct transition_table : mpl::vector<
            //     Start,     Event,            Next,      Action,                         Guard
            a_row <Follower,  Timeout,          Candidate, &rp::startElections>,
             _row <Candidate, Timeout,          Leader>,
              row <Leader,    GotAppendEntries, Follower,  &rp::giveUpLeadership,  &rp::isHisTermHigher >
        > {};
        
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM& fsm, int state)
        {
            std::cout << "No transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }

        inline int getTimeout() {return timeout_;}
        inline void setTimeout(int timeout) {timeout_ = timeout;}

        private:
            int timeout_;
            Receiver& r_;
            ServerState ss_;

    };

    // Pick a backend
    typedef msm::back::state_machine<raftProtocol_> raftProtocol;
    // Declare the run() method for the FSM
    void run(Receiver& r, ServerState& ss);

} // End of namespace
#endif