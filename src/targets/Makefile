include ../../setup.make

OBJS = HKTank.o TargetCyl.o TargetSphere.o TargetSphereTrunc.o DGBox.o
INCFILES = HKTank.h TargetCyl.h TargetSphere.h TargetSphereTrunc.h DGBox.h

LIBDIR= ../../lib
INCDIR= ../../include/
#CXX= g++

CXXFLAGS += -Werror -Wall -g -I$(INCDIR)

CXXFLAGS +=$(shell root-config --cflags)

INC :=$(addprefix $(INCDIR),$(INCFILES))
LIB :=$(LIBDIR)/libFluxTargets.a
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
