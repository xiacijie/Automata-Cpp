#pragma once
#include "FiniteAutomataBase.h"
#include <map>
#include <unordered_map>

using namespace std;

class DFA : public FiniteAutomataBase {
public:
    DFA() : FiniteAutomataBase() {}

    void addTransition(uint32_t fromState, char letter, uint32_t toState) override;
    void removeTransition(uint32_t fromState, char letter, uint32_t toState) override;
    bool isAdjacent(uint32_t fromState, uint32_t toState) override;
    bool hasTransitionFrom(uint32_t state) override;
    bool hasTransitionTo(uint32_t state) override;
    bool hasTransition(uint32_t fromState, char letter) override;
    string toString();

    uint32_t getTargetState(uint32_t fromState, char letter);

    /**
     * Complete the DFA so that the transition is total
     * @param garbageState
     */
    void complete(uint32_t garbageState);
    bool isComplete();

    /**
     * Get the complement of the current DFA
     * @return
     */
    DFA *complement();

    /**
     * Intersection with another DFA
     * @param other
     * @return
     */
    DFA *intersect(DFA *other);

    /**
     * Minimize the current DFA so that it has minimal states
     */
    void minimize();

    /**
     * Remove states that are not reachable from the start state
     */
    void removeUnreachableStates();

    /**
     * Check the emptiness of the DFA
     * @return
     */
    bool emptinessCheck();

    /**
     * Check the universality of the DFA
     * @return
     */
    bool universalityCheck();


    unordered_map<char, uint32_t>& getTransitions(uint32_t fromState);

private:
    unordered_map<uint32_t, unordered_map<char, uint32_t>> _transitionTable;
};


