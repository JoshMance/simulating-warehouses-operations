CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

all: experiment

experiment: model.o experiment.o
	$(CXX) $(CXXFLAGS) $^ -o $@

model.o: model.cpp order.cpp picker.cpp model.hpp
	$(CXX) $(CXXFLAGS) -c model.cpp -o model.o

order.o: order.cpp order.hpp
	$(CXX) $(CXXFLAGS) -c order.cpp -o order.o

picker.o: picker.cpp picker.hpp
	$(CXX) $(CXXFLAGS) -c picker.cpp -o picker.o

experiment.o: experiment.cpp model.hpp
	$(CXX) $(CXXFLAGS) -c experiment.cpp -o experiment.o

clean:
	rm -f experiment *.o