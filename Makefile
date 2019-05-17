SOURCES=$(wildcard *.cc)                                                                                                                         
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
CXX=g++
PROGRAM=mythreadpool
CXXFLAGS = -O2 -g -Wall -std=c++11 -fmessage-length=0
LDFLAGS = -lpthread

$(PROGRAM) : $(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(CXXFLAGS) $(LDFLAGS)
$(OBJECTS):$(SOURCES)

.PHONY : clean
clean :
	rm -rf $(OBJECTS) $(USE_EVENT_PROGRAM) $(USE_COND_PROGRAM)