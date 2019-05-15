FC=ftn
FCFLAGS=-g -dynamic 
CXX=CC
CXXFLAGS=-g -dynamic
C=cc
CFLAGS=-g -dynamic
TARGET=fc_simple
LDFLAGS=-lstdc++
DEBUG=-g

OBJS=fcapi.o capi.o

$(TARGET) : $(OBJS)
	$(FC) -o $(TARGET) main.F90 $(OBJS) $(LDFLAGS)
capi.o: capi.cc capi.h
	$(CXX) -c $(CXXFLAGS) capi.cc
fcapi.o: fcapi.F90
	$(FC) -c $(FCFLAGS) fcapi.F90
%.o: %.F90
	$(f90comp) -c $(switch) $<

.PHONY : clean
clean:
	rm -f *.o
	rm -f fcapi.mod
	rm -f capi_mod.mod
