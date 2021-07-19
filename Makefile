IDIR := /usr/include
CXXFLAGS = -I $(IDIR)

LDIR := /usr/lib
LDFLAGS= -L$(LDIR) -lnvfm

manageFabric: manageFabric.o
                $(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
                -@rm -f manageFabric.o
                -@rm -f manageFabric
