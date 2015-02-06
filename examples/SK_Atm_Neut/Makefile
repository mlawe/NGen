OBJS = AtmFluxGen.o UpMuFluxGen.o necardatm.o HonFlxTau.o HonFlx11Tau.o HonFlxUpMu.o HonFlx11UpMu.o
#tauvect.o  necardne.o necardne2.o 
#HonFlx.o HonFlx11New.o TargetCyl.o NuanceWriter.o RootWriter.o HKTank.o TargetSphere.o TargetSphereTrunc.o nevectap.o  necardatm.o necardne.o 
HK = HKflux
SK = SKflux
EV=evtRate
CRS=NeutCRS
ELCRS=NeutELCRS
LB=lboost
F=FERMI

SKOBJS=fortZBSOpen.o fortZBSClose.o ZBSWriter.o AtmRootWriter.o #nevectap_12b.o neventap_12b.o
#HKOBJS=nevectap.o  

PROGS = $(SK)

CXX= g++

FORT= g77

LD= g++


CXXFLAGS += -Werror -Wall -g

CXXFLAGS +=$(shell root-config --cflags)
FORTFLAGS +=-ffixed-line-length-none
FINCPATHS +=-I$(NEUT_ROOT)/inc 
FORTFLAGS +=-c -g -O -fno-backslash -fno-automatic -finit-local-zero -fno-second-underscore -fugly-complex  -fno-globals -fdollar-ok -fpic -Df2cFortran  -DSITE_HAS_NEUT  -ffortran-bounds-check $(FINCPATHS) 
LDFLAGS += -Werror -Wall 
#NEUTLIBS = -lneutcore -lnuceff -lneutcore -lpartnuck -lskmcsvc -lnuccorrspl -ltauola -lzbsfns -lspecfunc_5.3.2 
NEUTLIBS = -lneutcore -lnuceff -lneutcore -lpartnuck -lskmcsvc -lnuccorrspl -ltauola -lzbsfns 
NEUTLIBSPATH = -L$(NEUT_ROOT)/lib/Linux_pc/ 
SKOFLLIBS = -lgeom -lsklib -liolib -lmon -lzbs -llibrary  
SKOFLLIBPATH = -L$(SKOFL_ROOT)/lib
CINCPATHS += -I$(NEUT_ROOT)/include -I/home/kachulis/atmflux/base/include
FLUXBASELIBS += -lFluxWriters -lFluxTables -lFluxTargets -lFluxCore 
FLUXBASELIBSPATH += -L/home/kachulis/atmflux/base/lib
CXXFLAGS +=$(CINCPATHS)


FORTRANLIBSPATH = -L/usr/lib/gcc/x86_64-redhat-linux/3.4.6/
#CERNLIBS =  -lkernlib -ljetset74  -lpdflib804 -lmathlib -lpacklib  
CERNLIBS = `cernlib pdflib804 jetset74 photos202 mathlib packlib kernlib` 

CERNLIBSPATH = -L$(CERN)/2005/lib
LDFLAGS += -g -Dlinux -D__linux__ $(NEUTLIBSPATH)  $(CERNLIBSPATH) $(FORTRANLIBSPATH)   $(FLUXBASELIBSPATH) $(NEUTLIBS) $(CERNLIBS)  $(NUMLIBS) $(FLUXBASELIBS)  -ffixed-line-length-none -Df2cFortran -lg2c -lgfortran $(shell root-config --ldflags) $(shell root-config --libs)

.PHONY: clean distclean

all: $(PROGS)


%o::%c 
	$(CXX) $(CXXFLAGS) -c $? -o $@
%o::%F
	$(FORT) $(FORTFLAGS) -c -O0 $? -o $@

$(HK): $(OBJS) $(HKOBJS) $(HK).o
	$(LD) $^ $(LDFLAGS) $(SKOFLLIBPATH) $(SKOFLLIBS) $(LDFLAGS) -o $@
	@echo "$@ done"

$(SK): $(OBJS) $(SKOBJS) $(SK).o
	$(LD) $^ $(LDFLAGS) $(SKOFLLIBPATH) $(SKOFLLIBS) $(LDFLAGS)   -o $@
	@echo "$@ done"

$(EV): $(EVOBJS) $(EV).o
	$(LD) $^ $(SKOFLLIBPATH) $(SKOFLLIBS) $(LDFLAGS)  -o $@
	@echo "$@ done"

$(CRS): $(CRSOBJS) $(CRS).o
	$(LD) $^ $(SKOFLLIBPATH) $(SKOFLLIBS) $(LDFLAGS)  -o $@
	@echo "$@ done"

$(ELCRS): $(ELCRSOBJS) $(ELCRS).o
	$(LD) $^ $(SKOFLLIBPATH) $(SKOFLLIBS) $(LDFLAGS)  -o $@
	@echo "$@ done"

$(LB): $(LBOBJS) $(LB).o
	$(LD) $^ $(SKOFLLIBPATH) $(SKOFLLIBS) $(LDFLAGS)  -o $@
	@echo "$@ done"

$(F): $(FOBJS) $(F).o
	$(LD) $^ $(SKOFLLIBPATH) $(SKOFLLIBS) $(LDFLAGS)  -o $@
	@echo "$@ done"
clean:
	rm -f *.o

distclean: clean
	rm -f $(PROGS)






