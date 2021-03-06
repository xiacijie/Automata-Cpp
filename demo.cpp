#include "src/DFA.h"
#include "src/NFA.h"
#include <iostream>

using namespace std;

int main() {

    unordered_set<char> nfaAlphabet{'a','b',' '};
    NFA nfa;
    nfa.setAlphabet(nfaAlphabet);

    nfa.addState(0);
    nfa.addState(1);
    nfa.addState(2);


    nfa.setStartState(0);
    nfa.setAcceptState(2);

    nfa.addTransition(0, 'a', 1);
    nfa.addTransition(1, 'b', 2);

    nfa.addTransition(0, ' ', 1);

    cout << "***** The NFA *****" << endl;
    cout << nfa.toString() << endl;

    cout << "***** The DFA *****" << endl;
    DFA* dfa = nfa.convertToDFA();
    cout << dfa->toString() << endl;


    cout << "***** The minimized DFA *****" << endl;
    dfa->minimize();
    cout << dfa->toString() << endl;

    cout << "***** The complement of the DFA *****" << endl;
    DFA* comp = dfa->complement();
    cout << comp->toString() << endl;

    cout << "**** The intersection of the DFA with its complement ****" << endl;
    DFA* intersect = dfa->intersect(comp);
    cout << intersect->toString() << endl;


    intersect->minimize();
    cout << "*** Minimization of the intersection ***" << endl;
    cout << intersect->toString() << endl;

    cout << "**** Is the intersect empty? ****" << endl;
    string answer;

    if (intersect->emptinessCheck()) {
        answer = "Yes";
    }
    else {
        answer = "No";
    }

    cout << answer << endl;


    cout << "**** Is the intersect universal? ****" << endl;
    string answer1;

    if (intersect->universalityCheck()) {
        answer1 = "Yes";
    }
    else {
        answer1 = "No";
    }

    cout << answer1 << endl;




    delete dfa;
    delete comp;
    delete intersect;

    return 0;
}