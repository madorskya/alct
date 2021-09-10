# Makefile for ALCT model, based on VPP library.
# Alexander Madorsky, University of Florida/Physics. 

LIBNAME    = alct

#these options are passed from top-level make, unless we are on top-level
ifeq ($(MAKELEVEL), 0)
include ../parameters.mk
endif


## Directory structure
SDIR = .
LIBDIR = ../lib

## VPP library
VPPDIR = ../vpp_long_rval_no_cgen/src/

## other includes and libs

LOCINC     = -I. -I$(VPPDIR)

## options for compiling

CCOPT      = \
-Wall $(OPTIM) $(LOCINC) $(DRVALS) $(VERGEN) $(ALCTTYPE) $(ME11TYPE) \
$(MIRROR) $(CHIP) $(FLAVOR) $(KILLEQUAL) $(RED_LAT) $(DAQFORM) $(FW_VERSION) 

## get objects and targets from source file names

#CCFILES  := $(wildcard *.cpp)
CCFILES := \
alct.cpp \
collider.cpp \
crc.cpp \
dlls.cpp \
hit_counters.cpp \
jtag.cpp \
outfifo.cpp \
patterner_rl.cpp \
promoter_rl.cpp \
stage0.cpp \
stage1_rl.cpp \
synchro.cpp \
loopback.cpp \
ecc16_decoder.cpp \
ecc32_encoder.cpp \
shower.cpp \
$(VPPDIR)/vlib.cpp


ifeq ($(DAQFORM), -DDAQ01)
CCFILES += \
RawHitMemory.cpp \
fifo.cpp \
rollbackraw.cpp \
bestfifo.cpp \
dav.cpp 
endif

ifeq ($(DAQFORM), -DDAQ06)
CCFILES += \
daq.cpp \
dav06.cpp \
daq_memory.cpp \
delay_line.cpp \
delay_line_valor.cpp \
daq_fifo.cpp \
l1a_maker.cpp \
adaq.cpp \
al1a_maker.cpp \
dual_memory.cpp
endif

ifeq ($(FLAVOR), -DLX100)
CCFILES += gbtx.cpp
endif

ifeq ($(FLAVOR), -DLX150T)
CCFILES += gtp_tux.cpp
endif

ifdef TRIGWP 
CCFILES += trigger_wp.cpp
CCOPT += -DTRIGWP
else
CCFILES += trigger_rl.cpp
endif

OBJECTS   = $(CCFILES:.cpp=.o)
HFILES    = $(CCFILES:.cpp=.h)

## rules
all: $(LIBDIR)/lib$(LIBNAME).a

#depends: $(CCFILES) $(HFILES)
#	@echo Making dependencies...
#	@for ccf in $(CCFILES); do ($(CC) $(CCOPT) -MM $(LOCINC) $$ccf >> depends); done;:

%.o: %.cpp
	$(CC) $(CCOPT) -c $< -o $*.o

$(LIBDIR)/lib$(LIBNAME).a: $(OBJECTS)
	ar -rv $(LIBDIR)/lib$(LIBNAME).a $(OBJECTS)

alct.o: alct.cpp alct.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h dlls.h jtag.h daq.h \
  daq_memory.h delay_line.h daq_fifo.h l1a_maker.h crc.h dav06.h \
  outfifo.h trigger_rl.h stage0.h stage1_rl.h patterner_rl.h collider.h \
  promoter_rl.h synchro.h hit_counters.h loopback.h ecc16_decoder.h \
  ecc32_encoder.h Makefile ../parameters.mk

collider.o: collider.cpp collider.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h alctver.h Makefile ../parameters.mk

crc.o: crc.cpp crc.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

gbtx.o: gbtx.cpp gbtx.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

gtp_tux.o: gtp_tux.cpp gtp_tux.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

dlls.o: dlls.cpp dlls.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

hit_counters.o: hit_counters.cpp hit_counters.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h alctver.h Makefile ../parameters.mk

jtag.o: jtag.cpp jtag.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

outfifo.o: outfifo.cpp outfifo.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

patterner_rl.o: patterner_rl.cpp patterner_rl.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

promoter_rl.o: promoter_rl.cpp promoter_rl.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

stage0.o: stage0.cpp stage0.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

stage1_rl.o: stage1_rl.cpp stage1_rl.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h \
  patterner_rl.h Makefile ../parameters.mk

synchro.o: synchro.cpp synchro.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

loopback.o: loopback.cpp loopback.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

ecc16_decoder.o: ecc16_decoder.cpp ecc16_decoder.h alctver.h $(VPPDIR)/vlib.h \
  $(VPPDIR)/vmac.h Makefile ../parameters.mk

ecc32_encoder.o: ecc32_encoder.cpp ecc32_encoder.h alctver.h $(VPPDIR)/vlib.h \
  $(VPPDIR)/vmac.h Makefile ../parameters.mk

vlib.o: vlib.cpp $(VPPDIR)/vlib.h Makefile ../parameters.mk

daq.o: daq.cpp daq.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h daq_memory.h alctver.h delay_line.h \
  daq_fifo.h l1a_maker.h crc.h dav06.h Makefile ../parameters.mk

dav06.o: dav06.cpp dav06.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

daq_memory.o: daq_memory.cpp daq_memory.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

delay_line.o: delay_line.cpp delay_line.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

daq_fifo.o: daq_fifo.cpp daq_fifo.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

l1a_maker.o: l1a_maker.cpp l1a_maker.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h delay_line.h \
  alctver.h Makefile ../parameters.mk

adaq.o: adaq.cpp adaq.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h daq_memory.h alctver.h delay_line.h \
  daq_fifo.h al1a_maker.h dual_memory.h crc.h Makefile ../parameters.mk

al1a_maker.o: al1a_maker.cpp al1a_maker.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h dual_memory.h \
  alctver.h Makefile ../parameters.mk

dual_memory.o: dual_memory.cpp dual_memory.h alctver.h $(VPPDIR)/vlib.h $(VPPDIR)/vmac.h Makefile ../parameters.mk

trigger_rl.o: trigger_rl.cpp trigger_rl.h alctver.h $(VPPDIR)/vlib.h stage0.h \
  $(VPPDIR)/vmac.h stage1_rl.h patterner_rl.h collider.h promoter_rl.h Makefile ../parameters.mk

show:
ifdef TRIGWP
	@echo 'TRIGWP = $(TRIGWP)'
endif
	@echo 'DAQFORM = $(DAQFORM)'
	@echo $(VERGEN)
	@echo $(DRVALS)
	@echo $(ALCTTYPE)
	@echo $(ME11TYPE)
	@echo $(CCOPT)
	@echo $(MAKEFLAGS)
	@echo $(CCFILES)
	@echo $(HFILES)

clean :
	rm -f *~
	rm -f *.o
	rm -f $(VPPDIR)/*.o
	rm -r $(LIBDIR)/lib$(LIBNAME).a
	rm -f depends



