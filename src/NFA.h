#pragma once
#include "src/FiniteAutomataBase.h"
#include "src/DFA.h"
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>


class NFA : public FiniteAutomataBase {
public:
    void addTransition(uint32_t fromState, char letter, uint32_t toState) override;
    void removeTransition(uint32_t fromState, char letter, uint32_t toState) override;

    bool isAdjacent(uint32_t fromState, uint32_t toState) override;
    bool hasTransitionFrom(uint32_t state) override;
    bool hasTransitionTo(uint32_t state) override;
    bool hasTransition(uint32_t fromState, char letter) override;
    string toString();

    DFA* convertToDFA();

private:
    // (from-state, statement, {to-states})
    unordered_map<uint32_t,unordered_map<char, unordered_set<uint32_t>>> _transitionTable;
    void powerSetGenerationHelper(unordered_set<uint32_t>::iterator it,
                                    set<uint32_t> tempSet,
                                    set<set<uint32_t>>& powerSetSet,
                                    map<set<uint32_t>, uint32_t>& powerSetMap,
                                    uint32_t& newState,
                                    DFA* automata);

    set<uint32_t> epsilonClosure(uint32_t state);
    set<uint32_t> epsilonClosure(unordered_set<uint32_t>& states);
};


