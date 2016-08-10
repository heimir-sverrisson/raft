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

namespace msm = boost::msm;
namespace mpl = boost::mpl;

namespace raft_fsm {
    // Events
    struct Timeout {};
    struct HeardServerWithHigherTerm {};
    struct GotMajorityVote {};
    struct GotVoteResponse {
        GotVoteResponse(VoteResponse vr) : vr_(vr){};
        VoteResponse vr_;
    };
    struct GotAppendEntries {
        GotAppendEntries(AppendEntries ae) : ae_(ae){};
        AppendEntries ae_;
    };
    struct GotRequestVote {
        GotRequestVote(RequestVote rv) : rv_(rv){};
        RequestVote rv_;
    };
    struct Completed{
        enum Result{ok,noGood};
    };
    
    // Front end for the main protocol state machine
    struct RaftProtocol_ : public msm::front::state_machine_def<RaftProtocol_>
    {
        typedef RaftProtocol_ rp; // Shorthand for table

        RaftProtocol_(Receiver& r, ServerState& ss) : 
            timeout_(Config::readPeriod), r_(r), ss_(ss){};
      
        template<class Event, class FSM>
        void on_entry(Event const& evt, FSM& fsm){
            BOOST_LOG_TRIVIAL(info) << "Entering the raftProtocol state machine";
        }

        template<class Event, class FSM>
        void on_exit(Event const& evt, FSM& fsm){
            BOOST_LOG_TRIVIAL(info) << "Exiting the raftProtocol state machine";
        }
      
        // This State in the main FSM is a submachine itself
        struct Candidate_ : public msm::front::state_machine_def<Candidate_>
        {
            typedef Candidate_ cd; // Shorten transition table entries
      
            template<class Event, class FSM>
            void on_entry(Event const& evt, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Entering Candidate";
                // The fsm here is the main fsm
                // Copy our data members
                ssp_ = &fsm.ss_;
                fsmp_ = &fsm;
            }
      
            template<class Event, class FSM>
            void on_exit(Event const& evt, FSM& fsm){
                fsm.timeout_ = Config::readPeriod;
                BOOST_LOG_TRIVIAL(info) << "Exiting Candidate";
            }
            // Substates
            struct SetRandomTimeout : public msm::front::state<>
            {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm& fsm) const {
                    BOOST_LOG_TRIVIAL(info) << "Entering SetRandomTimeout";
                    // Set the timeout to a random value
                    int tMin = Config::readPeriod/2;
                    int t = tMin + rand() % tMin;
                    (fsm.fsmp_)->timeout_ = t;
                }
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    BOOST_LOG_TRIVIAL(info) << "Exiting SetRandomTimeout";
                }
            };
            struct WaitForVoteResponse : public msm::front::state<>
            {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm& fsm) const {
                    BOOST_LOG_TRIVIAL(info) << "Entering WaitForVoteResponse";
                }
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    BOOST_LOG_TRIVIAL(info) << "Exiting WaitForVoteResponse";
                }
            };

            // Pseudo exit state to Follower
            struct ExitToFollower : public msm::front::exit_pseudo_state<GotRequestVote>{
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm& fsm) const {
                    BOOST_LOG_TRIVIAL(info) << "Entering ExitToFollower";
                }
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    BOOST_LOG_TRIVIAL(info) << "Exiting ExitToFollower";
                }
            };
            // Pseudo exit state to Leader
            struct ExitToLeader : public msm::front::exit_pseudo_state<GotVoteResponse>{
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm& fsm) const {
                    BOOST_LOG_TRIVIAL(info) << "Entering ExitToLeader";
                }
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    BOOST_LOG_TRIVIAL(info) << "Exiting ExitToLeader";
                }
            };

            // Transition actions
            void sendMyVote(const GotRequestVote& evt){
                int myTerm = ssp_->getTerm();
                int hisTerm = evt.rv_.getTerm();
                int candidateId = evt.rv_.getCandidateId();
                Sender s;
                if(hisTerm > myTerm) {
                    BOOST_LOG_TRIVIAL(info) << "Voting for: " << candidateId;
                    s.sendVoteResponse(*ssp_, candidateId, 1); // He gets our vote
                } else {
                    BOOST_LOG_TRIVIAL(info) << "Not voting for: " << candidateId;
                    s.sendVoteResponse(*ssp_, candidateId, 0);
                }
            }

            void startElections(const Timeout& t){
                BOOST_LOG_TRIVIAL(info) << "Starting elections!";
                // Increment my term
                ssp_->setTerm(ssp_->getTerm() + 1);
                Sender s;
                s.sendRequestVote(*ssp_);
                VoteCollector& vc = ssp_->getVoteCollector();
                vc.clearVotes();
                vc.storeVote(ssp_->getMyId()); // Vote for myself
            }

            void takeLeadership(const GotVoteResponse& vr){
                BOOST_LOG_TRIVIAL(info) << "Taking Leadership";
            }

            // Guard conditions
            bool gotEnoughVotes(const GotVoteResponse& vr){
               VoteCollector& vc = ssp_->getVoteCollector();
               int vote = vr.vr_.getVoteGranted();
                if( vote == 1){
                    vc.storeVote(vr.vr_.getNodeId());
                }
               return vc.isElected(); 
            }

            bool isHisTermHigher(const GotRequestVote& evt){
                int myTerm = ssp_->getTerm();
                int hisTerm = evt.rv_.getTerm();
                if (hisTerm > myTerm){
                    return true;
                } else {
                    return false;
                }
            }

            typedef SetRandomTimeout initial_state;

            // States of the submachine
            struct transition_table : mpl::vector<
                //     Start,               Event,           Next,                Action,                 Guard
                a_row <SetRandomTimeout,    Timeout,         WaitForVoteResponse, &cd::startElections>,
                  row <SetRandomTimeout,    GotRequestVote,  ExitToFollower,      &cd::sendMyVote,        &cd::isHisTermHigher>,
                 _row <WaitForVoteResponse, Timeout,         SetRandomTimeout>,
                  row <WaitForVoteResponse, GotVoteResponse, ExitToLeader,        &cd::takeLeadership,    &cd::gotEnoughVotes >
            > {};

            // Replaces the default no-transition response.
            template <class FSM,class Event>
            void no_transition(Event const& e, FSM& fsm, int state)
            {
                BOOST_LOG_TRIVIAL(error) << "No transition from state " << state
                    << " on event " << typeid(e).name();
            }

            // Local data for submachine Candidate
            ServerState* ssp_;
            RaftProtocol_ *fsmp_;
        }; // end of struct Candidate_

        typedef msm::back::state_machine<Candidate_> Candidate;
        
        // States of the FSM
        struct Follower : public msm::front::state<> 
        {
            template<class Event, class FSM>
            void on_entry(Event const& evt, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Entering the Follower state";
                fsm.timeout_ = Config::readPeriod;
            }
            template<class Event, class FSM>
            void on_exit(Event const& timeout, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Exiting Follower";
            }            
        };

        struct Leader : public msm::front::state<> {
            template<class Event, class FSM>
            void on_entry(Event const& evt, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Entering Leader";
                fsm.timeout_ = Config::leaderPeriod;
            }
            template<class Event, class FSM>
            void on_exit(Event const& evt, FSM& fsm){
                BOOST_LOG_TRIVIAL(info) << "Exiting Leader";
                fsm.timeout_ = Config::readPeriod;
            }
        };

        typedef Follower initial_state;
        // Action
        void giveUpLeadership(const GotAppendEntries& evt){
            // Safe to lose this message if I'm not the leader 
            BOOST_LOG_TRIVIAL(error) << "Somebody send me AppendEntries - but I thought I was the leader";
        }
        void stopElections(const GotAppendEntries& evt){
            // Safe to lose this message if we are in the middle of elections
            BOOST_LOG_TRIVIAL(error) << "Somebody sent me AppendEntries - aborting elections";
        }

        void sendHeartbeat(const Timeout& evt){
            Sender s;
            s.sendAppendEntries(ss_);
        }
        void processAppendentries(const GotAppendEntries& evt){
            BOOST_LOG_TRIVIAL(info) << "AppendEntries: " << evt.ae_.to_string();
        }

        void sendMyVote(const GotRequestVote& evt){
            int myTerm = ss_.getTerm();
            int hisTerm = evt.rv_.getTerm();
            int candidateId = evt.rv_.getCandidateId();
            Sender s;
            if(hisTerm > myTerm) {
                BOOST_LOG_TRIVIAL(info) << "Voting for: " << candidateId;
                s.sendVoteResponse(ss_, candidateId, 1); // He gets our vote
            } else {
                BOOST_LOG_TRIVIAL(info) << "Not voting for: " << candidateId;
                s.sendVoteResponse(ss_, candidateId, 0);
            }
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

        bool isHisTermHigher(const GotRequestVote& evt){
            int myTerm = ss_.getTerm();
            int hisTerm = evt.rv_.getTerm();
            if (hisTerm > myTerm){
                BOOST_LOG_TRIVIAL(info) << "Lost leadership!";
                return true;
            } else {
                BOOST_LOG_TRIVIAL(info) << "I'm still the leader";
                return false;
            }
        }

        bool votedForMe(const GotVoteResponse& evt) { return true;}

        // Transition Table for protocol main state machine
        // DO NOT RENAME THIS VARIABLE !!!
        struct transition_table : mpl::vector<
            //     Start,                      Event,            Next,      Action,                 Guard
             _row <Follower,                   Timeout,          Candidate >,
            a_row <Follower,                   GotRequestVote,   Follower,  &rp::sendMyVote >,
            a_row <Follower,                   GotAppendEntries, Follower,  &rp::processAppendentries >,
              row <Candidate,                  GotAppendEntries, Follower,  &rp::stopElections,     &rp::isHisTermHigher >,
              row <Leader,                     GotAppendEntries, Follower,  &rp::giveUpLeadership,  &rp::isHisTermHigher >,
              row <Leader,                     GotRequestVote,   Follower,  &rp::sendMyVote,        &rp::isHisTermHigher >,
            a_row <Leader,                     Timeout,          Leader,    &rp::sendHeartbeat >,
             _row<Candidate::exit_pt
                 <Candidate_::ExitToFollower>, GotRequestVote,          Follower>,
             _row<Candidate::exit_pt
                 <Candidate_::ExitToLeader>,   GotVoteResponse,          Leader>
        > {};
        
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM& fsm, int state)
        {
            BOOST_LOG_TRIVIAL(error) << "No transition from state " << state
                << " on event " << typeid(e).name();
        }

        inline int getTimeout() {return timeout_;}
        inline void setTimeout(int timeout) {timeout_ = timeout;}
        
        // State variables
        int timeout_;
        Receiver& r_;
        ServerState& ss_;
    };

    // Define the backend
    typedef msm::back::state_machine<RaftProtocol_> RaftProtocol;
    
    // Declare the run() method for the FSM
    void run(Receiver& r, ServerState& ss);

} // End of namespace
#endif