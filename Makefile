SRC_DIR := src

all:	demo.cpp src/FiniteAutomataBase.cpp src/DFA.cpp src/NFA.cpp
	g++ -o demo demo.cpp src/FiniteAutomataBase.cpp src/DFA.cpp src/NFA.cpp -I.

clean:
	rm test