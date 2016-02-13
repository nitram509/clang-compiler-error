appname := example_constructor_check_wrong_return_code

CXX := clang++
CXXFLAGS := -std=c++11 -Wall -O1

srcfiles := $(shell find . -name "*.cxx")
objects  := $(patsubst %.cxx, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(srcfiles) $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend

include .depend