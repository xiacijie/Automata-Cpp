#include "NFA.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>

using namespace std;


DFA* NFA::convertToDFA() {
    DFA* automata = new DFA();

    map<set<uint32_t>, uint32_t> powerSetMap;
    set<uint32_t> tempSet;

    uint32_t newState = 0;

    set<set<uint32_t>> powerSetSet;
    powerSetGenerationHelper(_states.begin() ,tempSet, powerSetSet, powerSetMap, newState, automata);

    automata->setStartState(powerSetMap[epsilonClosure(_startState)]);

    for (const auto& dfaFromStateSet: powerSetSet) {

        for (const auto& alpha: _alphabet) {
            if (alpha == ' ')
                continue;

            set<uint32_t> dfaToStateSet;
            for (const auto& s: dfaFromStateSet) {
                if (hasTransition(s, alpha)) {
                    auto closure = epsilonClosure(_transitionTable[s][alpha]);
                    dfaToStateSet.insert(closure.begin(), closure.end());
                }
            }

            uint32_t dfaFromState = powerSetMap[dfaFromStateSet];
            uint32_t dfaToState = powerSetMap[dfaToStateSet];
            automata->addTransition(dfaFromState, alpha, dfaToState);
        }
    }

    set<uint32_t> emptySet;

    unordered_set<char> copyOfAlphabet;
    copyOfAlphabet.insert(_alphabet.begin(), _alphabet.end());
    const auto&it = copyOfAlphabet.find(' ');
    if (it != copyOfAlphabet.end())
        copyOfAlphabet.erase(it);

    automata->setAlphabet(copyOfAlphabet);

    return automata;
}

set<uint32_t> NFA::epsilonClosure(unordered_set<uint32_t>& states) {
    set<uint32_t> eClosure;
    for (const auto& s : states) {
        auto c = epsilonClosure(s);
        eClosure.insert(c.begin(), c.end());
    }

    return eClosure;
}

set<uint32_t> NFA::epsilonClosure(uint32_t state) {
    set<uint32_t> eClosure;
    queue<uint32_t> q;
    unordered_set<uint32_t> visited;

    eClosure.insert(state);
    q.push(state);

    while (!q.empty()) {
        uint32_t currentState = q.front();
        visited.insert(currentState);
        q.pop();

        if (hasTransitionFrom(currentState)) {
            for (const auto& t : _transitionTable[currentState]) {
                if (t.first == ' ') { // epsilon transition
                    for (const auto& s: t.second) {
                        if (visited.find(s) == visited.end()) {
                            eClosure.insert(s);
                            q.push(s);
                        }
                    }
                }
            }
        }

    }

    return eClosure;
}

void NFA::powerSetGenerationHelper(unordered_set<uint32_t>::iterator it,
                                   set<uint32_t> tempSet,
                                   set<set<uint32_t>>& powerSetSet,
                                   map<set<uint32_t>, uint32_t>& powerSetMap,
                                   uint32_t& newState,
                                   DFA* automata) {
    powerSetSet.insert(tempSet);
    powerSetMap[tempSet] = newState;
    automata->addState(newState);

    for (const auto& acceptState : _acceptStates) {
        if (tempSet.find(acceptState) != tempSet.end()) {
            automata->setAcceptState(newState);
            break;
        }
    }

    newState++;

    if (it == _states.end())
        return;

    for (auto localIt = it; localIt != _states.end(); localIt++) {
        tempSet.insert(*localIt);
        powerSetGenerationHelper(++it, tempSet, powerSetSet,powerSetMap, newState, automata);
        const auto& it1 = tempSet.find(*localIt);
        tempSet.erase(it1);
    }

}


bool NFA::hasTransitionFrom(uint32_t state) {
    const auto& it = _transitionTable.find(state);
    return it != _transitionTable.end();
}

bool NFA::hasTransitionTo(uint32_t state) {
    for (const auto& it : _transitionTable){
        bool any = any_of(it.second.begin(), it.second.end(), [=](const auto& it1) {
            return any_of(it1.second.begin(), it1.second.end(), [=](const auto& it2) {
                return it2 == state;
            });
        });

        if (any)
            return true;
    }

    return false;
}

bool NFA::hasTransition(uint32_t fromState, char letter) {
    if (!hasTransitionFrom(fromState))
        return false;

    return _transitionTable[fromState].find(letter) != _transitionTable[fromState].end();
}

bool NFA::isAdjacent(uint32_t fromState, uint32_t toState) {
    if (!hasTransitionFrom(fromState))
        return false;

    return any_of(_transitionTable[fromState].begin(), _transitionTable[fromState].end(), [=](const auto& it) {
        return any_of(it.second.begin(), it.second.end(), [=](const auto& it1) {
            return it1 == toState;
        });
    });
}

string NFA::toString() {
    stringstream ss;

    ss << "NFA:" << endl;
    ss << "|============|" << endl;
    ss << "Num of Total States: " << getNumStates() << endl;
    ss << "Size of the Alphabet: " << _alphabet.size() << endl;
    ss << "Start State: " << getStartState() << endl;
    ss << "Accept States: " << getNumAcceptingStates() << " States" << endl;
    for (const auto& elem: _acceptStates) {
        ss << elem << " ";
    }
    ss << endl;
    ss << "Transitions: " << endl;

    for (const auto& t: _transitionTable) {
        for (const auto& it : t.second) {

            ss << "( " << t.first << ", " << it.first << ", " << "{";

            int i = 0;
            for (const auto& it1: it.second) {
                ss << it1;
                if (i != it.second.size() - 1) {
                    ss <<", ";
                }
                i++;
            }

            ss << "} )";
        }

        ss << endl;
    }
    ss << "|============|" << endl;

    return ss.str();
}

void NFA::addTransition(uint32_t fromState, char letter, uint32_t toState) {
    assert(hasState(fromState) && "From State does not exist!\n");
    assert(hasState(toState) && "To State does not exist!\n");

    _transitionTable[fromState][letter].insert(toState);
}

void NFA::removeTransition(uint32_t fromState, char letter, uint32_t toState) {
    assert(hasState(fromState) && "From State does not exist!\n");
    assert(hasState(toState) && "To State does not exist!\n");

    if (hasTransition(fromState, letter)) {
        const auto& it = _transitionTable[fromState][letter].find(toState);
        if (it == _transitionTable[fromState][letter].end()) {
            assert(false && "Transition does not exit!\n");
        }

        _transitionTable[fromState][letter].erase(it);
    }

    assert(false && "Transition does not exit!\n");
}

