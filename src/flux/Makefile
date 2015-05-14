include ../../setup.make
OBJS = HonFlx11.o HonFlx.o BeamFlux.o T2HKFlux.o SNFlux.o
INCFILES = HonFlx11.h HonFlx.h BeamFlux.h T2HKFlux.h SNFlux.h

LIBDIR= ../../lib
INCDIR= ../../include/
#CXX= clang++

CXXFLAGS += -Werror -Wall -g -I$(INCDIR)

CXXFLAGS +=$(shell root-config --cflags)

INC :=$(addprefix $(INCDIR),$(INCFILES))
LIB :=$(LIBDIR)/libFluxTables.a
.PHONY: clean inc lib

all: $(LIB) $(INC)

inc: $(INC)

lib: $(LIB)

%o::%c 
	$(CXX) $(CXXFLAGS) -c $? -o $@

$(LIB): $(OBJS)
	ar rcs $@ $?


clean:
	rm -f *.o

$(INCDIR)%::%
	cp $^ $@
