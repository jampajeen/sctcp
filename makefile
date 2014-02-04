RELEASEDIR = released/bin/
PROGRAM = sctcp
#INCDIRS = -I./common
LIBDIRS = -L/usr/lib
LIBS = -lpthread -lrt
CXXSOURCES = $(wildcard *.cpp)
SRCS = 	$(CXXSOURCES)
CXXOBJECTS = $(SRCS:.cpp=.o) pugixml.o
CXXFLAGS = -DOS_LINUX -DM_GENERIC_INT32 -m64 -fPIC -O -Wall -g
CXX = g++
LDFLAGS = $(LIBDIRS) $(LIBS)

all: $(PROGRAM)
$(PROGRAM): $(CXXOBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(CXXOBJECTS)
pugixml.o:
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -c -o pugixml.o pugixml/pugixml.cpp
clean:
	$(RM) -f $(CXXOBJECTS) $(PROGRAM) *.so
run:
	./$(PROGRAM)
