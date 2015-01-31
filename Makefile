

.PHONY: libs includes clean

all: includes libs

libs: 
	(cd src/core && $(MAKE) lib)
	(cd src/flux && $(MAKE) lib)
	(cd src/targets && $(MAKE) lib)
	(cd src/writers && $(MAKE) lib)

includes:
	(cd src/core && $(MAKE) inc)
	(cd src/flux && $(MAKE) inc)
	(cd src/targets && $(MAKE) inc)
	(cd src/writers && $(MAKE) inc)

clean:
	(cd src/core && $(MAKE) clean)
	(cd src/flux && $(MAKE) clean)
	(cd src/targets && $(MAKE) clean)
	(cd src/writers && $(MAKE) clean)
	rm lib/*
	rm include/*
