CXX=g++

LD=g++

NGEN = ../../

TimeTestOBJS= TimeFlux.o TimeGen.o

CXXFLAGS += -Werror -Wall -g

CXXFLAGS +=$(shell root-config --cflags)

CXXFLAGS += -I$(NGEN)/include
LDFLAGS += -Werror -Wall

LDFLAGS +=-L$(NGEN)/lib/

LDFLAGS +=-lFluxWriters -lFluxTables -lFluxTargets -lFluxCore

LDFLAGS += -g $(shell root-config --ldflags) $(shell root-config --libs)



.PHONY: clean distclean


%o::%c
	$(CXX) $(CXXFLAGS) -c $? -o $@

TimeTest: TimeTest.o $(TimeTestOBJS)
	$(LD) $^ $(LDFLAGS) $(LDFLAGS) -o $@
	@echo "$@ done"

clean:
	rm -f *.o
