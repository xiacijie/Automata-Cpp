#include "FiniteAutomataBase.h"
#include <iostream>

using namespace std;

void FiniteAutomataBase::addState(uint32_t state) {
    assert(_states.find(state) == _states.end() && "State already exists!\n");
    _states.insert(state);
}

void FiniteAutomataBase::removeState(uint32_t state) {
    auto it = _states.find(state);
    assert(it != _states.end() && "state does not exists!\n");
    assert(!hasTransitionFrom(state) && "Cannot remove a state which already has transitions from it!\n");
    assert(!hasTransitionTo(state) && "Cannot remove a state which already has transitions to it!\n");

    _states.erase(it);

    if (isAcceptState(state)) {
        _acceptStates.erase(_acceptStates.find(state));
    }
}

void FiniteAutomataBase::setStartState(uint32_t state) {
    assert(hasState(state) && "Does not have this state!\n");
    _startState = state;
}

void FiniteAutomataBase::setAcceptState(uint32_t state) {
    assert(hasState(state) && "Does not have this state!\n");
    _acceptStates.insert(state);
}

void FiniteAutomataBase::setAlphabet(unordered_set<char> &alphabet) {
    _alphabet.clear();
    _alphabet.insert(alphabet.begin(), alphabet.end());
}