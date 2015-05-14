CXX=g++

LD=g++

NGEN = ../../
NEUT_ROOT = /home/other/mlawe/t2k/NEUT/neut_5.3.3_v1r21

#OBJS= SNFlux.o SNGen.o NeutGen.o
OBJS= NeutGen.o

FORT=gfortran
FORTFLAGS +=-ffixed-line-length-none
FINCPATHS +=-I$(NEUT_ROOT)/inc
FORTFLAGS +=-c -g -O -fno-backslash -fno-automatic -finit-local-zero -fno-second-underscore -fugly-complex  -fno-globals -fdollar-ok -fpic -Df2cFortran  -DSITE_HAS_NEUT  -ffortran-bounds-check $(FINCPATHS)

NEUTLIBS = -lneutcore -lnuceff  -lpartnuck -lskmcsvc -lnuccorrspl -lspecfunc_5.3.3
NEUTLIBSPATH = -L$(NEUT_ROOT)/lib/Linux_pc/
CINCPATHS += -I$(NEUT_ROOT)/include

#FORTRANLIBSPATH = -L/usr/lib/gcc/x86_64-redhat-linux/4.4.7/
CERNLIBS = `cernlib pdflib804 jetset74 photos202 mathlib packlib kernlib`
CERNLIBSPATH = -L$(CERN)/2005/lib

CXXFLAGS += -Werror -Wall -g
CXXFLAGS +=$(shell root-config --cflags)
CXXFLAGS += -I$(NGEN)/include
CXXFLAGS +=$(CINCPATHS)

LDFLAGS += -Werror -Wall
LDFLAGS += -L$(NGEN)/lib/
LDFLAGS += $(NEUTLIBSPATH) $(CERNLIBSPATH) $(NEUTLIBS) $(CERNLIBS)
LDFLAGS += -lFluxWriters -lFluxTables -lFluxTargets -lFluxCore
LDFLAGS += -g $(shell root-config --ldflags) $(shell root-config --libs)
LDFLAGS += -ffixed-line-length-none -Df2cFortran -lgfortran

.PHONY: clean distclean

%o::%c
	$(CXX) $(CXXFLAGS) -c $? -o $@

TitusSN: TitusSN.o $(OBJS)
	$(LD) $^ $(LDFLAGS) $(LDFLAGS) -o bin/$@
	@echo "$@ done"

clean:
	rm -f *.o