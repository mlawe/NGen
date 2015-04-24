OBJS =   HonFlxTau.o HonFlx11Tau.o HonFlxUpMu.o HonFlx11UpMu.o NeutRootWriter.o
#tauvect.o  necardne.o necardne2.o 
#HonFlx.o HonFlx11New.o TargetCyl.o NuanceWriter.o RootWriter.o HKTank.o TargetSphere.o TargetSphereTrunc.o nevectap.o  necardatm.o necardne.o 
HK = HKflux

PROGS = $(HK)_neut $(HK)_genie

CXX= g++

FORT= g77

LD= g++

NEUT=$(NEUT_ROOT)
SKOFL=$(SKOFL_ROOT)
NGEN=/home/kachulis/atmflux/base

LOCAL=/disk2/usr5/kachulis

CXXFLAGS += -Werror -Wall -g

CXXFLAGS +=$(shell root-config --cflags)

CXXFLAGS += -I$(NGEN)/include



LDFLAGS += -Werror -Wall

LDFLAGS +=-L$(NGEN)/lib/

LDFLAGS +=-lFluxWriters -lFluxTables -lFluxTargets -lFluxCore



FORTRANLIBSPATH = -L/usr/lib/gcc/x86_64-redhat-linux/3.4.6/

LDFLAGS += -g $(shell root-config --ldflags) 	

#stuff for GENIE
LOG4CPPLIBS = -llog4cpp
LOG4CPPLIBSPATH =-L$(LOCAL)/log4cpp/lib
LHAPDFLIBS=-lLHAPDF
LHAPDFLIBSPATH=-L$(LOCAL)/local/lib
PYTHIA6LIBS=-lPythia6 #-llund
PYTHIA6LIBSPATH=-L$(LOCAL)/pythia6/v6_424/lib

#stuff for NEUT
NEUTLIBS = -lneutcore -lnuceff -lneutcore -lpartnuck -lskmcsvc -lnuccorrspl -ltauola -lzbsfns -lspecfunc_5.3.2 
#NEUTLIBS = -lneutcore -lnuceff -lneutcore -lpartnuck -lskmcsvc -lnuccorrspl -ltauola -lzbsfns 
NEUTLIBSPATH = -L$(NEUT)/lib/Linux_pc/ 
SKOFLLIBS = -lgeom -lsklib -liolib -lmon -lzbs -llibrary  
SKOFLLIBPATH = -L$(SKOFL)/lib

CERNLIBS = `cernlib pdflib804 jetset74 photos202 mathlib packlib kernlib` 


OBJSNEUT = $(OBJS) NeutGen.o UpMuFluxGen.o necardatm.o
CXXFLAGSNEUT = $(CXXFLAGS) -I$(NEUT)/include -DNEUT
LDFLAGSNEUT = $(LDFLAGS) $(NEUTLIBSPATH)  $(CERNLIBSPATH) $(FORTRANLIBSPATH) $(NEUTLIBS) $(CERNLIBS) $(SKOFLLIBPATH) $(SKOFLLIBS)
LDFLAGSNEUT +=$(NEUTLIBSPATH)  $(CERNLIBSPATH) $(FORTRANLIBSPATH) $(NEUTLIBS) $(CERNLIBS) $(SKOFLLIBPATH) $(SKOFLLIBS)
LDFLAGSNEUT += -g $(shell root-config --ldflags) $(shell root-config --libs)
LDFLAGSNEUT += $(FORTRANLIBSPATH) -ffixed-line-length-none -Df2cFortran -lg2c -lgfortran

OBJSGENIE = $(OBJS) GenieFluxGen.o
CXXFLAGSGENIE = $(CXXFLAGS) -I$(LOCAL)/local/include/GENIE -DGENIE
LDFLAGSGENIE = $(LDFLAGS) $(shell genie-config --libs) $(shell root-config --libs) -lGeom -lEG -lEGPythia6 $(PYTHIA6LIBSPATH) $(PYTHIA6LIBS) $(LHAPDFLIBSPATH) $(LHAPDFLIBS) -lxml2 $(LOG4CPPLIBSPATH) $(LOG4CPPLIBS)   
LDFLAGSGENIE += $(FORTRANLIBSPATH) -ffixed-line-length-none -Df2cFortran -lg2c -lgfortran	




.PHONY: clean distclean

all: $(PROGS)


%o::%c 
	$(CXX) $(CXXFLAGS) -c $? -o $@

NeutGen.o: NeutGen.cc
	$(CXX) $(CXXFLAGSNEUT) -c $? -o $@

UpMuFluxGen.o: UpMuFluxGen.cc
	$(CXX) $(CXXFLAGSNEUT) -c $? -o $@

GenieFluxGen.o: GenieFluxGen.cc
	$(CXX) $(CXXFLAGSGENIE) -c $? -o $@
GenieGen.o: GeniFluxGen.cc
	$(CXX) $(CXXFLAGSGENIE) -c $? -o $@

HKflux_NEUT.o: HKflux.cc
	$(CXX) $(CXXFLAGSNEUT) -c $? -o $@

HKflux_GENIE.o: HKflux.cc
	$(CXX) $(CXXFLAGSGENIE) -c $? -o $@
%o::%F
	$(FORT) $(FORTFLAGS) -c -O0 $? -o $@

$(HK)_neut: $(HK)_NEUT.o $(OBJSNEUT) 
	$(LD) $^ $(LDFLAGSNEUT) -o bin/$@
	@echo "$@ done"

$(HK)_genie: $(HK)_GENIE.o $(OBJSGENIE)
	$(LD) $^ $(LDFLAGSGENIE) -o bin/$@

clean:
	rm -f *.o

distclean: clean
	rm -f $(PROGS)






