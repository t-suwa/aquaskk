/* -*- C++ -*-

   Generic State Machine Library for C++.

   Copyright (c) 2006-2008, Tomotaka SUWA <t.suwa@mac.com>

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   Neither the name of the authors nor the names of its contributors
   may be used to endorse or promote products derived from this
   software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef GenericStateMachine_h
#define GenericStateMachine_h

#include <cassert>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <functional>

namespace statemachinecxx_sourceforge_jp {
    // ======================================================================
    // event types
    // ======================================================================
    enum EventTypes {
        EXIT_EVENT = -3,
        INIT_EVENT = -2,
        ENTRY_EVENT = -1,
	PROBE = 0,
        USER_EVENT = 1
    };

    // ======================================================================
    // event
    // ======================================================================
    template <class ParamType>
    class GenericEvent {
        int signal_;
        ParamType param_;

    public:
        GenericEvent() {}
        GenericEvent(int signal) : signal_(signal) {}
        GenericEvent(int signal, const ParamType& param) : signal_(signal), param_(param) {}

        operator int() const	    { return signal_; }
        void SetSignal(int signal)  { signal_ = signal;}

        const ParamType& Param() const		{ return param_; }
        void SetParam(const ParamType& arg)	{ param_ = arg; }

	bool IsSystem() const	{ return signal_ < USER_EVENT; }
	bool IsUser() const	{ return !IsSystem(); }

        static GenericEvent& Probe()	{ static GenericEvent evt(PROBE);	return evt; }
        static GenericEvent& Entry()    { static GenericEvent evt(ENTRY_EVENT); return evt; }
        static GenericEvent& Exit()     { static GenericEvent evt(EXIT_EVENT);  return evt; }
        static GenericEvent& Init()     { static GenericEvent evt(INIT_EVENT);  return evt; }
    };

    // ======================================================================
    // state
    // ======================================================================
    template <class StateContainer>
    class GenericState {
	typedef typename StateContainer::Handler Handler;

        enum StateType {
            UNKNOWN,
            SUPER,
            INITIAL,
            TRANSITION,
            SAVE_HISTORY,
            SHALLOW_HISTORY,
            DEEP_HISTORY,
	    FORWARD,
	    DEEP_FORWARD,
            DEFER_EVENT,
	    CLEAR_HISTORY
        };

        StateType type_;
        Handler handler_;

        GenericState(StateType type, Handler handler) : type_(type), handler_(handler) {}

    public:
        GenericState() : type_(UNKNOWN), handler_(0) {}
	GenericState(Handler handler) : type_(SUPER), handler_(handler) {}

        operator Handler() const { return handler_; }

        bool IsSuper() const		{ return type_ == SUPER; }
        bool IsInitial() const          { return type_ == INITIAL; }
        bool IsTransition() const       { return type_ == TRANSITION; }
        bool IsSaveHistory() const      { return type_ == SAVE_HISTORY; }
        bool IsShallowHistory() const   { return type_ == SHALLOW_HISTORY; }
        bool IsDeepHistory() const      { return type_ == DEEP_HISTORY; }
        bool IsForward() const		{ return type_ == FORWARD; }
        bool IsDeepForward() const	{ return type_ == DEEP_FORWARD; }
        bool IsDeferEvent() const       { return type_ == DEFER_EVENT; }
        bool IsClearHistory() const     { return type_ == CLEAR_HISTORY; }

        static GenericState Initial(Handler handler)            { return GenericState(INITIAL, handler); }
        static GenericState Transition(Handler handler)         { return GenericState(TRANSITION, handler); }
        static GenericState SaveHistory()                       { return GenericState(SAVE_HISTORY, 0); }
        static GenericState ShallowHistory(Handler handler)     { return GenericState(SHALLOW_HISTORY, handler); }
        static GenericState DeepHistory(Handler handler)        { return GenericState(DEEP_HISTORY, handler); }
        static GenericState Forward(Handler handler)		{ return GenericState(FORWARD, handler); }
        static GenericState DeepForward(Handler handler)	{ return GenericState(DEEP_FORWARD, handler); }
        static GenericState DeferEvent()                        { return GenericState(DEFER_EVENT, 0); }
        static GenericState ClearHistory()                      { return GenericState(CLEAR_HISTORY, 0); }
    };

    // ======================================================================
    // state history
    // ======================================================================
    template <typename Handler>
    class GenericStateHistory {
        typedef std::pair<Handler, Handler> ShallowDeepPair;
	typedef std::pair<Handler, ShallowDeepPair> Entry;
	typedef std::list<Entry> History;
	typedef typename History::iterator HistoryIterator;

        History history_;

	enum HistoryTypes { SHALLOW, DEEP };

	class Equal : public std::unary_function<Entry, bool> {
	    Handler key_;

	public:
	    Equal(const Handler arg) : key_(arg) {}

	    bool operator()(const Entry& entry) const {
		return key_ == entry.first;
	    }
	};

        HistoryIterator lookup(const Handler key) {
	    return std::find_if(history_.begin(), history_.end(), Equal(key));
        }

        const Handler get(const Handler key, HistoryTypes type) {
            HistoryIterator iter = lookup(key);

            if(iter == history_.end()) return 0;

	    ShallowDeepPair& pair = iter->second;

	    return type == SHALLOW ? pair.first : pair.second;
        }

    public:
        void Save(const Handler key, const Handler shallow, const Handler deep) {
            HistoryIterator iter = lookup(key);
	    ShallowDeepPair pair = std::make_pair(shallow, deep);

            if(iter != history_.end()) {
                iter->second = pair;
            } else {
		history_.push_front(std::make_pair(key, pair));
            }
        }

	void Clear(const Handler key) {
            HistoryIterator iter = lookup(key);

	    if(iter != history_.end()) {
		history_.erase(iter);
	    }
	}

        const Handler Shallow(const Handler key) {
            return get(key, SHALLOW);
        }

        const Handler Deep(const Handler key) {
            return get(key, DEEP);
        }
    };

    // ======================================================================
    // deferred event
    // ======================================================================
    template <typename Handler, typename Event>
    class GenericDeferEvent {
	typedef std::queue<Event> Defer;
        typedef std::pair<Handler, Defer> Entry;
	typedef std::list<Entry> Queue;
	typedef typename Queue::iterator QueueIterator;

        Queue incoming_;
	Queue outgoing_;

        class Equal : public std::unary_function<Entry, bool> {
	    const Handler key_;

	public:
	    Equal(Handler arg) : key_(arg) {}

            bool operator()(const Entry& entry) const {
		return key_ == entry.first;
            }
        };

	QueueIterator find(const Handler key, Queue& queue) {
	    return std::find_if(queue.begin(), queue.end(), Equal(key));
	}

    public:
        void Enqueue(const Handler key, const Event& event) {
	    QueueIterator iter = find(key, incoming_);

	    if(iter == incoming_.end()) {
		incoming_.push_front(std::make_pair(key, Defer()));
		iter = incoming_.begin();
	    }

	    iter->second.push(event);
        }

	void Commit(const Handler key) {
	    QueueIterator iter = find(key, incoming_);

	    if(iter != incoming_.end()) {
		outgoing_.push_front(*iter);
		incoming_.erase(iter);
	    }
	}

        bool Dequeue(const Handler key, Event& event) {
	    QueueIterator iter = find(key, outgoing_);

            if(iter == outgoing_.end()) return false;
	    
	    if(iter->second.empty()) {
		outgoing_.erase(iter);
		return false;
	    }
		
	    event = iter->second.front();
	    iter->second.pop();

	    return true;
        }
    };

    // ======================================================================
    // empty inspector
    // ======================================================================
    template <typename Handler, typename Event>
    struct EmptyInspector {
	void operator()(const Handler handler, const Event& event) {}
    };

    // ======================================================================
    // state machine
    // ======================================================================
    template <typename StateContainer,
	      template <typename Handler, typename Event>
	      class Inspector = EmptyInspector>
    class GenericStateMachine {
    public:
	typedef typename StateContainer::State State;
	typedef typename StateContainer::Event Event;
	typedef typename StateContainer::Handler Handler;

    private:
        StateContainer container_;
	Inspector<Handler, Event> inspector_;

        Handler top_;
        Handler active_;
        Handler prior_;
	int selfTransitionCount_;

        GenericStateHistory<Handler> history_;
        GenericDeferEvent<Handler, Event> queue_;

	Event defer_;

	typedef typename std::vector<Handler> Path;
	typedef typename Path::iterator PathIterator;

	Path path_;

	// ------------------------------------------------------------
	// invoke state function
	// ------------------------------------------------------------
	State invoke(const Handler handler, const Event& event) {
	    inspector_(handler, event);
	    return (container_.*handler)(event);
	}

	// ------------------------------------------------------------
	// system event trigger
	// ------------------------------------------------------------
        State getSuperState(const Handler handler)	{ return invoke(handler, Event::Probe()); }
        State entryAction(const Handler handler)	{ return invoke(handler, Event::Entry()); }
        State initialTransition(const Handler handler)	{ return invoke(handler, Event::Init()); }
        State exitAction(const Handler handler) {
            State result = invoke(handler, Event::Exit());

            if(result.IsSaveHistory()) {
                assert(prior_ != 0 && "*** Shallow history not found ***");
                history_.Save(handler, prior_, active_);
            }

	    queue_.Commit(handler);

            prior_ = handler;

            return result;
        }

	// ------------------------------------------------------------
	// initial transition trigger
	// ------------------------------------------------------------
        void initialize(const State& target) {
            State state;

            active_ = target;

            for(state = initialTransition(active_); state.IsInitial() || state.IsShallowHistory();
                state = initialTransition(active_)) {
		assert(state != active_ && "*** An infinte loop detected. Check INIT_EVENT ***");
                assert((getSuperState(state) == active_ || getSuperState(state) == getSuperState(active_))
                       && "*** Initial transition must go to same or one level deep ***");

                if(state.IsShallowHistory()) {
                    Handler shallow = history_.Shallow(active_);
                    if(!shallow) {
                        history_.Save(active_, state, 0); // first time
                        active_ = state;
                    } else {
                        active_ = shallow;
                    }
                } else {
                    active_ = state;
                }

                entryAction(active_);
            }

            assert(state != 0 && state.IsSuper() &&
		   "*** Initial transition must be ended by returning super state ***");
        }

	// ------------------------------------------------------------
	// transition trigger
	// ------------------------------------------------------------
        void transition(const Handler source, const Handler target) {
            assert(target != top_ && "*** Transition to TopState is not allowed ***");

	    Handler tmp;

	    // exit to source
	    for(tmp = active_, prior_ = 0; tmp != source; tmp = getSuperState(tmp)) {
		exitAction(tmp);
	    }

	    // exit to LCA(Least Common Ancestor) and record the path to the target
	    do {
		path_.clear();
		path_.push_back(target);

		// (a) self transition
		if(source == target) {
		    assert(++ selfTransitionCount_ < 3 && "*** Too many self transitions. ***");
		    exitAction(source);
		    break;
		}

		// (b) go into substate(one level)
		Handler targetSuper = getSuperState(target);
		if(source == targetSuper) {
		    break;
		}

		// (c) balanced transition
		Handler sourceSuper = getSuperState(source);
		if(sourceSuper == targetSuper) {
		    exitAction(source);
		    break;
		}

		// (d) leave from substate(one level)
		if(sourceSuper == target) {
		    exitAction(source);
		    path_.clear();
		    break;
		}

		// (e) go into substate(multiple level)
		path_.push_back(targetSuper);
		for(tmp = getSuperState(targetSuper); tmp != 0; tmp = getSuperState(tmp)) {
		    if(source == tmp) {
			break;
		    }
		    path_.push_back(tmp);
		}
		if(tmp != 0) break;

		exitAction(source);

		struct check {
		    static bool exist(Path& path, const Handler handler) {
			PathIterator iter = std::find(path.begin(), path.end(), handler);
			if(iter != path.end()) {
			    path.erase(iter, path.end());
			    return true;
			}
			return false;
		    }
		};

		// (f) unbalanced transition
		if(check::exist(path_, sourceSuper)) {
		    break;
		}

		// (g) leave from substate(multiple level)
		for(tmp = sourceSuper; tmp != 0; tmp = getSuperState(tmp)) {
		    if(check::exist(path_, tmp)) {
			break;
		    }
		    exitAction(tmp);
		}
		if(tmp != 0) break;

		assert(0 && "*** Invalid state transition form detected ***");
	    } while(0);

	    // go into the target
	    while(!path_.empty()) {
		entryAction(path_.back());
		path_.pop_back();
	    }
        }

    public:
        GenericStateMachine() : top_(&StateContainer::TopState), active_(0) {}
        GenericStateMachine(const StateContainer& src) : container_(src), top_(&StateContainer::TopState), active_(0) {}

        ~GenericStateMachine() {
	    if(!container_.ExitOnDestruct()) return;

            for(Handler tmp = active_; tmp != 0; tmp = getSuperState(tmp)) {
                exitAction(tmp);
            }
        }

        void Start() {
            assert(active_ == 0 && "*** You can not call Start() twice ***");

            initialize(State::Initial(top_));
        }

        void Dispatch(const Event& event) {
            if(!active_) Start();

            State next;

	    selfTransitionCount_ = 0;
            for(Handler source = active_; source != 0; source = next) {
                next = invoke(source, event);

		if(next.IsDeferEvent()) {
		    queue_.Enqueue(source, event);
		    break;
		}

		if(next.IsClearHistory()) {
		    history_.Clear(source);
		    break;
		}

		if(next.IsTransition() || next.IsDeepHistory() || next.IsForward() || next.IsDeepForward()) {
		    State target = next;

		    if(next.IsDeepHistory() || next.IsDeepForward()) {
			target = history_.Deep(next);
			assert(target != 0 && "*** Deep history not found ***");
		    }

		    transition(source, target);
		    initialize(target);

		    if(next.IsForward() || next.IsDeepForward()) {
			next = target;
			continue;
		    }

		    while(queue_.Dequeue(source, defer_)) {
			Dispatch(defer_);	// recursion
		    }
		    break;
		}

		assert(!next.IsShallowHistory() && !next.IsSaveHistory() && !next.IsInitial() &&
		       "*** Invalid state detected ***");
            }
        }

	const Handler CurrentState() const {
	    return active_;
	}

        const StateContainer& Container() const {
            return container_;
        }

        bool IsChildOf(const Handler state) {
            for(Handler current = active_; current != top_; current = getSuperState(current)) {
                if(current == state) {
                    return true;
                }
            }

            return false;
        }
    };

    // ======================================================================
    // base state container
    // ======================================================================
    template <typename StateContainer, typename ParamType = int>
    struct BaseStateContainer {
	typedef GenericState<StateContainer> State;
	typedef GenericEvent<ParamType> Event;
	typedef State (StateContainer::*Handler)(const Event&);

	virtual ~BaseStateContainer() {}

	// derived class must define this method
	virtual const Handler InitialState() const = 0;

	virtual bool ExitOnDestruct() const { return true; }

	virtual State TopState(const Event& event) {
	    switch(event) {
	    case INIT_EVENT:
		return State::Initial(InitialState());
	    }
	    return 0;
	}
    };

    // ======================================================================
    // state container traits
    // ======================================================================
    template <typename T>
    struct StateContainerTraits {
	typedef typename T::State State;
	typedef typename T::Event Event;
	typedef typename T::Handler Handler;
    };
}

#endif	// INC__GenericStateMachine__
