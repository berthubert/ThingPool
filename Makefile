CXXFLAGS:=-std=gnu++17 -Wall -O -MMD -MP -pthread

PROGRAMS = testrunner

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.d

testrunner: testrunner.o
	$(CXX) -std=gnu++17 $^ -o $@ 

