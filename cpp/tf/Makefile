# Makefile for ALCT test fixtures
# Alexander Madorsky, University of Florida/Physics. 


#these options are passed from top-level make, unless we are on top-level
ifeq ($(MAKELEVEL), 0)
include ../parameters.mk
endif

## Directory structure
HDIR = ../src
LIBDIR = ../lib
BDIR = ../bin
LIBNAME    = $(wildcard $(LIBDIR)/*.a)

VPPDIR = ../vpp_long_rval_no_cgen/src/
## other includes and libs
LOCINC     = -I. -I$(HDIR) -I$(VPPDIR)

## options for compiling

CCOPT      = \
-Wall $(OPTIM) $(LOCINC) $(DRVALS) $(VERGEN) $(ALCTTYPE) $(ME11TYPE) \
$(MIRROR) $(CHIP) $(FLAVOR) $(KILLEQUAL) $(RED_LAT) $(DAQFORM) $(FW_VERSION)

## get objects and targets from source file names

#CCFILES  := $(wildcard *.cpp)
#CCFILES := alcttf.cpp promoter_rltf.cpp
CCFILES := alcttf.cpp trigger_rltf.cpp adaqtf.cpp text_reverse.cpp
ifeq ($(DAQFORM), -DDAQ01)
#CCFILES += fifotf.cpp
endif

ifeq ($(DAQFORM), -DDAQ06)
CCFILES += daqtf.cpp
endif

ifdef TRIGWP
CCOPT += -DTRIGWP
endif

OBJECTS   = $(CCFILES:.cpp=.o)
TARGETS   = $(CCFILES:.cpp=.exe)
#TARGETS = alcttf.exe
TARGROOT  = $(CCFILES:.cpp=)

## rules
all: $(TARGETS)

#depends: $(CCFILES)
#	@echo Making dependencies...
#	@for tff in $(TARGROOT); do (echo $$tff.exe: $$tff.o $(LIBNAME) >> depends); done;:
#	@for ccf in $(CCFILES); do ($(CC) $(CCOPT) -MM $(LOCINC) $$ccf >> depends); done;:

%.o: %.cpp
	$(CC) $(CCOPT) -c $<

%.exe: %.o
	$(CL) $(CCOPT) -o $(BDIR)/$@ $^

alcttf.exe: alcttf.o ../lib/libalct.a

trigger_rltf.exe: trigger_rltf.o ../lib/libalct.a

adaqtf.exe: adaqtf.o ../lib/libalct.a

text_reverse.exe: text_reverse.o ../lib/libalct.a

daqtf.exe: daqtf.o ../lib/libalct.a

alcttf.o: alcttf.cpp ../src/alct.h ../src/alctver.h $(VPPDIR)/vlib.h \
  $(VPPDIR)/vmac.h ../src/dlls.h ../src/jtag.h ../src/daq.h \
  ../src/daq_memory.h ../src/delay_line.h ../src/daq_fifo.h \
  ../src/l1a_maker.h ../src/crc.h ../src/dav06.h ../src/outfifo.h \
  ../src/trigger_rl.h ../src/stage0.h ../src/stage1_rl.h \
  ../src/patterner_rl.h ../src/collider.h ../src/promoter_rl.h \
  ../src/synchro.h ../src/hit_counters.h ../src/loopback.h \
  ../src/ecc16_decoder.h ../src/ecc32_encoder.h Makefile ../src/Makefile ../parameters.mk

trigger_rltf.o: trigger_rltf.cpp ../src/trigger_rl.h ../src/alctver.h \
  $(VPPDIR)/vlib.h ../src/stage0.h $(VPPDIR)/vmac.h ../src/stage1_rl.h \
  ../src/patterner_rl.h ../src/collider.h ../src/promoter_rl.h \
  $(VPPDIR)/tfmac.h Makefile ../src/Makefile ../parameters.mk

adaqtf.o: adaqtf.cpp ../src/adaq.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h \
  ../src/daq_memory.h ../src/alctver.h ../src/delay_line.h \
  ../src/daq_fifo.h ../src/al1a_maker.h ../src/dual_memory.h ../src/crc.h Makefile ../src/Makefile ../parameters.mk

text_reverse.o: text_reverse.cpp

daqtf.o: daqtf.cpp ../src/daq.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h \
  ../src/daq_memory.h ../src/alctver.h ../src/delay_line.h \
  ../src/daq_fifo.h ../src/l1a_maker.h ../src/crc.h ../src/dav06.h Makefile ../src/Makefile ../parameters.mk

show:
	$(CC) --version
	@echo $(VERGEN)
	@echo $(DRVALS)
	@echo $(ALCTTYPE)
	@echo $(ME11TYPE)
	@echo $(OPTIM)
	@echo $(LIBNAME)
	@echo $(OBJECTS)
	@echo $(TARGETS)

clean :
	rm -f *~
	rm -f *.o
	rm -f $(TARGETS)
	rm -f $(BDIR)/*.exe
	rm -f depends














