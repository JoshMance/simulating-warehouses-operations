CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

all: model-tester

model-tester: model.o model-tester.o
	$(CXX) $(CXXFLAGS) $^ -o $@

model.o: model.cpp model.hpp
	$(CXX) $(CXXFLAGS) -c model.cpp -o model.o

model-tester.o: model-tester.cpp model.hpp
	$(CXX) $(CXXFLAGS) -c model-tester.cpp -o model-tester.o

clean:
	rm -f model-tester *.o