all:	test.cpp FiniteAutomataBase.cpp DFA.cpp NFA.cpp
	g++ -o test test.cpp FiniteAutomataBase.cpp DFA.cpp NFA.cpp -I.

clean:
	rm test