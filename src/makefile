CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

all: experiment

experiment: model.o experiment.o location.o order.o picker.o
	$(CXX) $(CXXFLAGS) $^ -o $@

model.o: model.cpp location.hpp order.hpp picker.hpp
	$(CXX) $(CXXFLAGS) -c model.cpp 

location.o: location.cpp 
	$(CXX) $(CXXFLAGS) -c location.cpp

order.o: order.cpp 
	$(CXX) $(CXXFLAGS) -c order.cpp

picker.o: picker.cpp 
	$(CXX) $(CXXFLAGS) -c picker.cpp 

experiment.o: experiment.cpp 
	$(CXX) $(CXXFLAGS) -c experiment.cpp

clean:
	rm -f experiment *.o