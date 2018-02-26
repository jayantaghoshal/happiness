/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#ifndef DFSM_H
#define DFSM_H
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <vector>

class DFsm_State;
class DFsm_Transition;

using StatePointer      = std::shared_ptr<DFsm_State>;
using TransitionPointer = std::shared_ptr<DFsm_Transition>;

using StateMap        = std::map<int, StatePointer>;
using TransitionMap   = std::multimap<int, TransitionPointer>;
using TransitionPair  = std::pair<int, TransitionPointer>;
using TransitionQueue = std::queue<TransitionPointer>;

// This class is the main class which hold the current state and makes the transition
class DFsm_Main
{

public:
    DFsm_Main(int initState = -1);

    // create a state.
    void createState(int                   state,
                     int                   parentState   = 0,
                     std::function<void()> entryFunction = nullptr,
                     std::function<void()> exitFunction  = nullptr,
                     std::function<void()> bodyFunction  = nullptr);

    // Add entry, exit and body to state.
    void addStateEntry(int state, std::function<void()> entryFunction);
    void addStateExit(int state, std::function<void()> exitFunction);
    void addStateBody(int state, std::function<void()> bodyFunction);

    // register the upmost initial state.
    void registerMainInitState(int initState);

    // register the upmost initial state in a state.
    void addInitState(int inState, int initState);

    // register transition in between two states with guard.
    void createTransition(int fromState, int toState, std::function<bool()> guard = nullptr);

    // execute transition, i.e. switch between two states
    void executeTransition(int state1, int state2);

    // execute transition if it is in a state that has this transition.
    void transitionToState(int toState);

    // This will execute the current state's body.
    void executeBody();

    // This will execute the current or matching parent state's body.
    void executeBody(int state);

    // return if in state
    bool inState(int state);

private:
    // Updating history points.
    void updateHistoryPoint();

    // Change state depending on history and/or initial point exists.
    void changeState(TransitionPointer transition);

    void executeTransition(TransitionPointer transition);

    void queueTransition(TransitionPointer transition);

    bool inState(StatePointer state);

    StatePointer    currentState_;
    StateMap        stateMap_;
    TransitionMap   transitionList_;
    TransitionQueue transitionQueue_;
    bool            ongoingTransition_;
};

#endif // DFSM_H
