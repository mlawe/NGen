OBJS = RootWriter.o NuanceWriter.o
INCFILES = RootWriter.h NuanceWriter.h

LIBDIR= ../../lib
INCDIR= ../../include/
CXX= g++

CXXFLAGS += -Werror -Wall -g -I$(INCDIR)

CXXFLAGS +=$(shell root-config --cflags)

INC :=$(addprefix $(INCDIR),$(INCFILES))
LIB :=$(LIBDIR)/libFluxWriters.a
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
