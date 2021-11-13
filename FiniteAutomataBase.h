#pragma once
#include <memory>
#include <unordered_set>
#include <cassert>

using namespace std;


/**
* The base class for NFA and DFA
*/
class FiniteAutomataBase {
public:
    uint32_t getStartState() const { return _startState; }
    uint32_t getNumStates() { return _states.size(); }
    uint32_t getNumAcceptingStates() { return _acceptStates.size(); }

    void setAlphabet(unordered_set<char>& alphabet);

    bool hasState(uint32_t state) { return _states.find(state) != _states.end(); }
    bool isAcceptState(uint32_t state) { return _acceptStates.find(state) != _acceptStates.end(); }
    bool isStartState(uint32_t state) { return state == _startState; }

    void addState(uint32_t state);
    void removeState(uint32_t state);

    void setStartState(uint32_t state);
    void setAcceptState(uint32_t state);

    /**
    * To be implemented by NFA as well as DFA
    */
    virtual void addTransition(uint32_t fromState, char letter, uint32_t toState)=0;
    virtual void removeTransition(uint32_t fromState, char letter, uint32_t toState)=0;
    virtual bool isAdjacent(uint32_t fromState, uint32_t toState)=0;

    // state -> ...
    virtual bool hasTransitionFrom(uint32_t state)=0;

    // ... -> state
    virtual bool hasTransitionTo(uint32_t state)=0;

    virtual bool hasTransition(uint32_t fromState, char letter)=0;

    unordered_set<uint32_t>& getAcceptStates() { return _acceptStates; }
    unordered_set<uint32_t>& getStates() { return _states; }

protected:
    unordered_set<uint32_t> _states;
    uint32_t _startState;
    unordered_set<uint32_t> _acceptStates;
    unordered_set<char> _alphabet;
};

