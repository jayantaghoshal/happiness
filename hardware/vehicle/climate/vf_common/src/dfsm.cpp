/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dfsm.h"
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class DFsm_Entry;
class DFsm_Guard;
class DFsm_Body;
class DFsm_Exit;
class DFsm_Main;

using GuardPointer = shared_ptr<DFsm_Guard>;
using EntryPointer = shared_ptr<DFsm_Entry>;
using BodyPointer  = shared_ptr<DFsm_Body>;
using ExitPointer  = shared_ptr<DFsm_Exit>;
//==================== //

// Transition between states
class DFsm_Transition
{
public:
    static TransitionPointer create(StatePointer state1, StatePointer state2, GuardPointer guard = nullptr);
    StatePointer getState1();
    StatePointer getState2();
    GuardPointer getGuard();

private:
    DFsm_Transition(StatePointer state1, StatePointer state2, GuardPointer guard = nullptr);

    StatePointer State1_;
    StatePointer State2_;
    GuardPointer Guard_;
};
//==================== //
// This class takes a lambda function and execute it when enter state
class DFsm_Entry
{
public:
    static EntryPointer create(std::function<void()> entry);
    void execute(void);

private:
    DFsm_Entry(std::function<void()> entry);

    std::function<void()> entry_;
};
//==================== //
// This class takes a lambda function and execute it when called upon.
class DFsm_Body
{
public:
    static BodyPointer create(std::function<void()> body);
    void execute();

private:
    DFsm_Body(std::function<void()> body);

    std::function<void()> body_;
};
//==================== //
// This class takes a lambda function and execute it when called upon.
class DFsm_Guard
{
public:
    static GuardPointer create(std::function<bool()> guard);
    bool execute();

private:
    DFsm_Guard(std::function<bool()> guard);

    std::function<bool()> guard_;
};
//==================== //
// This class takes a lambda function and execute when exit state.
class DFsm_Exit
{
public:
    static ExitPointer create(std::function<void()> exit);
    void execute();

private:
    DFsm_Exit(std::function<void()> exit);

    std::function<void()> exit_;
};
//==================== //
// This class is the state class
class DFsm_State
{
public:
    static StatePointer create(StatePointer parent = nullptr,
                               int          state  = 0,
                               EntryPointer entry  = nullptr,
                               ExitPointer  exit   = nullptr,
                               BodyPointer  body   = nullptr);
    StatePointer getParent();
    int          getState();
    EntryPointer getEntry();
    ExitPointer  getExit();
    BodyPointer  getBody();

    void addEntry(EntryPointer entry);
    void addExit(ExitPointer exit);
    void addBody(BodyPointer body);

    // Update history point
    void UpdateHistoryPoint(StatePointer state);

    StatePointer getHistoryPoint();

    StatePointer getInitState();

    // register initial state in this state.
    void registerInitState(StatePointer state);

private:
    DFsm_State(StatePointer parent = nullptr,
               int          state  = 0,
               EntryPointer entry  = nullptr,
               ExitPointer  exit   = nullptr,
               BodyPointer  body   = nullptr);
    void changeState();

    StatePointer parent_;
    int          state_;
    EntryPointer entry_;
    BodyPointer  body_;
    ExitPointer  exit_;
    StatePointer historyState_;
    StatePointer initState_;
};

// ===  DFsm_Transition === //

TransitionPointer DFsm_Transition::create(StatePointer state1, StatePointer state2, GuardPointer guard)
{
    return TransitionPointer(new DFsm_Transition(state1, state2, guard));
}
//==================== //
DFsm_Transition::DFsm_Transition(StatePointer state1, StatePointer state2, GuardPointer guard)
    : State1_(state1)
    , State2_(state2)
    , Guard_(guard)
{
}
//==================== //
StatePointer DFsm_Transition::getState1()
{
    return State1_;
}
//==================== //
StatePointer DFsm_Transition::getState2()
{
    return State2_;
}
//==================== //
GuardPointer DFsm_Transition::getGuard()
{
    return Guard_;
}
//==================== //

// ===  DFsm_Entry === //

EntryPointer DFsm_Entry::create(std::function<void()> entry)
{
    return EntryPointer(new DFsm_Entry(entry));
}
//==================== //
DFsm_Entry::DFsm_Entry(std::function<void()> entry)
    : entry_(entry)
{
}
//==================== //
void DFsm_Entry::execute(void)
{
    entry_();
}
//==================== //

// ===  DFsm_Body === //

BodyPointer DFsm_Body::create(std::function<void()> body)
{
    return BodyPointer(new DFsm_Body(body));
}
//==================== //
DFsm_Body::DFsm_Body(std::function<void()> body)
    : body_(body)
{
}
//==================== //
void DFsm_Body::execute()
{
    body_();
}
//==================== //

// ===  DFsm_Guard === //

GuardPointer DFsm_Guard::create(std::function<bool()> guard)
{
    return GuardPointer(new DFsm_Guard(guard));
}
//==================== //
DFsm_Guard::DFsm_Guard(std::function<bool()> guard)
    : guard_(guard)
{
}
//==================== //
bool DFsm_Guard::execute()
{
    return guard_();
}
//==================== //

// ===  DFsm_Exit === //

ExitPointer DFsm_Exit::create(std::function<void()> exit)
{
    return ExitPointer(new DFsm_Exit(exit));
}
//==================== //
DFsm_Exit::DFsm_Exit(std::function<void()> exit)
    : exit_(exit)
{
}
//==================== //
void DFsm_Exit::execute()
{
    exit_();
}
//==================== //

// ===  DFsm_State === //

StatePointer DFsm_State::create(StatePointer parent, int state, EntryPointer entry, ExitPointer exit, BodyPointer body)
{
    return StatePointer(new DFsm_State(parent, state, entry, exit, body));
}

DFsm_State::DFsm_State(StatePointer parent, int state, EntryPointer entry, ExitPointer exit, BodyPointer body)
    : parent_(parent)
    , state_(state)
    , entry_(entry)
    , body_(body)
    , exit_(exit)
    , historyState_(nullptr)
{
}
//==================== //
StatePointer DFsm_State::getParent()
{
    return parent_;
}
//==================== //
int DFsm_State::getState()
{
    return state_;
}
//==================== //
void DFsm_State::addEntry(EntryPointer entry)
{
    entry_ = entry;
}
//==================== //
void DFsm_State::addExit(ExitPointer exit)
{
    exit_ = exit;
}
//==================== //
void DFsm_State::addBody(BodyPointer body)
{
    body_ = body;
}
//==================== //
EntryPointer DFsm_State::getEntry()
{
    return entry_;
}
//==================== //
ExitPointer DFsm_State::getExit()
{
    return exit_;
}
//==================== //
BodyPointer DFsm_State::getBody()
{
    return body_;
}
//==================== //
void DFsm_State::UpdateHistoryPoint(StatePointer state)
{
    historyState_ = state;
}
//==================== //
StatePointer DFsm_State::getHistoryPoint()
{
    return historyState_;
}
//==================== //
StatePointer DFsm_State::getInitState()
{
    return initState_;
}
//==================== //
void DFsm_State::registerInitState(StatePointer state)
{
    initState_ = state;
}
//==================== //

// ===  DFsm_Main === //

DFsm_Main::DFsm_Main(int initState)
    : currentState_(nullptr)
    , ongoingTransition_(false)
{
    StatePointer newState = DFsm_State::create(nullptr, initState);
    stateMap_.insert(std::make_pair(initState, newState));
    currentState_ = newState;
}
//==================== //
void DFsm_Main::createState(int                   state,
                            int                   parentState,
                            std::function<void()> entryFunction,
                            std::function<void()> exitFunction,
                            std::function<void()> bodyFunction)
{
    StatePointer parent = nullptr;
    EntryPointer entry  = nullptr;
    ExitPointer  exit   = nullptr;
    BodyPointer  body   = nullptr;

    if (0 != parentState)
    {
        parent = stateMap_[parentState];
    }
    if (nullptr != entryFunction)
    {
        entry = DFsm_Entry::create(entryFunction);
    }
    if (nullptr != exitFunction)
    {
        exit = DFsm_Exit::create(exitFunction);
    }
    if (nullptr != bodyFunction)
    {
        body = DFsm_Body::create(bodyFunction);
    }

    StatePointer newState = DFsm_State::create(parent, state, entry, exit, body);
    stateMap_.insert(std::make_pair(state, newState));
}
//==================== //
void DFsm_Main::addStateEntry(int state, std::function<void()> entryFunction)
{
    EntryPointer entry  = DFsm_Entry::create(entryFunction);
    StatePointer state1 = stateMap_[state];
    if (nullptr != state1)
        state1->addEntry(entry);
}
//==================== //
void DFsm_Main::addStateExit(int state, std::function<void()> exitFunction)
{
    ExitPointer  exit   = DFsm_Exit::create(exitFunction);
    StatePointer state1 = stateMap_[state];
    if (nullptr != state1)
        state1->addExit(exit);
}
//==================== //
void DFsm_Main::addStateBody(int state, std::function<void()> bodyFunction)
{
    BodyPointer  body   = DFsm_Body::create(bodyFunction);
    StatePointer state1 = stateMap_[state];
    if (nullptr != state1)
        state1->addBody(body);
}
//==================== //
void DFsm_Main::registerMainInitState(int initState)
{
    currentState_ = stateMap_[initState];
}
//==================== //
void DFsm_Main::addInitState(int inState, int initState)
{
    StatePointer state1 = stateMap_[inState];
    StatePointer state2 = stateMap_[initState];
    if (nullptr != state1 && nullptr != state2)
        state1->registerInitState(state2);
}
//==================== //
void DFsm_Main::createTransition(int fromState, int toState, std::function<bool()> guard)
{

    GuardPointer newGuard = nullptr;
    if (nullptr != guard)
    {
        newGuard = DFsm_Guard::create(guard);
    }
    StatePointer state1 = stateMap_[fromState];
    StatePointer state2 = stateMap_[toState];

    if (nullptr != state1 && nullptr != state2)
    {
        TransitionPointer transition = DFsm_Transition::create(state1, state2, newGuard);
        transitionList_.insert(TransitionPair(fromState, transition));
    }
}
//==================== //
void DFsm_Main::updateHistoryPoint()
{
    auto it = currentState_;

    while (nullptr != it->getParent())
    {
        it->getParent()->UpdateHistoryPoint(it);
        it = it->getParent();
    }
}
//==================== //
void DFsm_Main::changeState(TransitionPointer transition)
{

    // Traverse through history points to get the innermost current state
    // if exist. This will update when we enter sub-state.
    if (nullptr != transition->getState2()->getHistoryPoint())
    {

        auto it = transition->getState2()->getHistoryPoint();

        while (nullptr != it->getHistoryPoint())
        {
            it = it->getHistoryPoint();
        }
        currentState_ = it;
    } // Or..get the initial state if exist (also traverse through list)
    else if (nullptr != transition->getState2()->getInitState())
    {

        auto it = transition->getState2();

        while (nullptr != it->getInitState())
        {
            it = it->getInitState();
        };

        currentState_ = it;

        auto it2 = transition->getState2()->getInitState();
        while (nullptr != it2)
        {
            if (nullptr != it2.get()->getEntry())
                it2.get()->getEntry().get()->execute();
            it2 = it2.get()->getInitState();
        }

    } // Or..set current state if it is a plain state.
    else
    {
        currentState_ = transition->getState2();
    }
}
//==================== //
void DFsm_Main::executeTransition(TransitionPointer transition)
{

    if (inState(transition->getState1()->getState()))
    {

        // if guard fail don' make transition.
        if (nullptr != transition->getGuard())
        {
            if (!transition->getGuard()->execute())
                return;
        }

        // Execute Exit (lamnda function) if exist
        if (nullptr != transition.get()->getState1()->getExit())
        {
            transition.get()->getState1()->getExit()->execute();
        }

        // Change state.
        this->changeState(transition);

        // Update history point in parent(s) state.
        this->updateHistoryPoint();

        // Execute Entry (lamnda function) if exist
        if (nullptr != transition.get()->getState2()->getEntry())
        {
            transition.get()->getState2()->getEntry()->execute();
        }
    }
}
//==================== //
void DFsm_Main::executeTransition(int fromState, int toState)
{

    for (auto it = transitionList_.begin(); it != transitionList_.end(); ++it)
    {
        if (it->first == fromState && it->second.get()->getState2()->getState() == toState)
        {
            queueTransition(it->second);
            break;
        }
    }
}
//==================== //
void DFsm_Main::transitionToState(int toState)
{

    for (auto it = transitionList_.begin(); it != transitionList_.end(); ++it)
    {
        if (it->second.get()->getState2()->getState() == toState)
        {
            if (inState(it->first))
            {
                queueTransition(it->second);
                break;
            }
        }
    }
}
//==================== //
void DFsm_Main::queueTransition(TransitionPointer transition)
{

    if (false == ongoingTransition_)
    {
        ongoingTransition_ = true;
        executeTransition(transition);
    }
    else
    {
        transitionQueue_.push(transition);
        return;
    }

    while (!transitionQueue_.empty())
    {
        executeTransition(transitionQueue_.front());
        transitionQueue_.pop();
    }
    ongoingTransition_ = false;
}
//==================== //
void DFsm_Main::executeBody()
{
    // Execute Body (lamnda function) if exist
    if (nullptr != currentState_->getBody())
        currentState_->getBody()->execute();
}
//==================== //
void DFsm_Main::executeBody(int state)
{
    // Execute Body (lamnda function) if exist
    auto it = currentState_;

    do
    {
        if (it->getState() == state)
        {
            if (nullptr != it->getBody() && inState(it))
                it->getBody()->execute();
        }
        it = it->getParent();
    } while (nullptr != it);
}
//==================== //
bool DFsm_Main::inState(StatePointer state)
{

    auto it = currentState_;
    if (currentState_ == state)
        return true;

    while (nullptr != it->getParent())
    {
        if (it->getParent() == state)
            return true;
        it = it->getParent();
    }
    return false;
}
//==================== //
bool DFsm_Main::inState(int state)
{

    auto it = currentState_;
    if (currentState_.get()->getState() == state)
        return true;

    while (nullptr != it->getParent())
    {
        if (it->getParent().get()->getState() == state)
            return true;
        it = it->getParent();
    }
    return false;
}
//==================== //
