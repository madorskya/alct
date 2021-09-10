# generation and copy instructions for Verilog HDL files

#these options are passed from top-level make, unless we are on top-level
ifeq ($(MAKELEVEL), 0)
include ../parameters.mk
endif

FNAMES = \
Flip.v \
LyOneShot.v \
Patterner_rl.v \
Stage0.v \
jtag.v \
synchro.v \
loopback.v \
ecc16_decoder.v \
ecc32_encode.v \
outfifo.v 

#hitcnt.v 


ifeq ($(DAQFORM), -DDAQ01)
FNAMES += \
crc.v \
dav.v \
fifo.v
endif

ifeq ($(DAQFORM), -DDAQ06)
FNAMES += \
best_delay.v \
best_memory.v \
crcgen.v \
daq_06.v \
davgen.v \
l1a_fifo.v \
l1a_maker_.v \
raw_delay.v \
raw_memory.v \
l1a_bxn_fifo.v
endif

ifdef TRIGWP
FNAMES += trigger_wp.v
else
FNAMES += trigger_rl.v \
CheckNeighbors_rl.v \
promoter_rl.v \
collider.v \
Stage1_rl.v \
shower.v
endif

ifeq ($(FLAVOR), -DLX100)
FNAMES += gbtx.v
endif

ifeq ($(FLAVOR), -DLX150T)
FNAMES += gtp_tux.v
endif

VERDIR = verilog
ALCTVER = $(notdir $(subst .v,,$(subst alct,,$(wildcard $(VERDIR)/alct*.v))))

FLAV = $(FLAVOR)
FLAVOR_LX = $(findstring LX,$(FLAV))
FLAVOR_FULL =`expr substr $(FLAV) 3 10`

ifeq "$(strip $(FLAVOR_LX))" "$(strip LX)"
PRJDIR = $(PRJPFX)_$(FLAVOR_FULL)_$(ALCTVER)
else
ifeq ($(CHIP), -DS6)
PRJDIR = $(PRJPFX)_s6_$(ALCTVER)
else
PRJDIR = $(PRJPFX)$(ALCTVER)
endif
endif
FNAMES += alct$(ALCTVER).v
FILENAMES = $(addprefix $(VERDIR)/, $(FNAMES))
DIFF = diff
DIFFOPT = -w -I"// Time.*" -I"// Author.*" -I"// File.*"
OS   := $(shell uname -o)
TARGSUF  =.exe

MAINTF = ./alcttf$(TARGSUF)


.PHONY: show cp clean verilog diff

verilog: 
	@echo Generating Verilog HDL files...
	@cp $(MAINTF) $(VERDIR)
	@cd $(VERDIR); $(MAINTF); rm -f $(MAINTF)

show:
	@echo $(FLAV)
	@echo $(FLAVOR_LX)
	@echo $(FLAVOR_FULL)
	@echo $(PRJDIR)
	@echo $(FNAMES)
	@echo $(FILENAMES)
	@echo $(OS)
	@echo $(TARGSUF)
	@echo $(MAINTF)
	@pwd
	@ls $(VERDIR)
	@echo $(ALCTVER)
	@echo $(wildcard $(VERDIR)/alct*.v)
	@echo $(subst alct,,$(wildcard $(VERDIR)/alct*.v))
	@echo $(subst .v,,$(subst alct,,$(wildcard $(VERDIR)/alct*.v)))
	@echo $(notdir $(subst .v,,$(subst alct,,$(wildcard $(VERDIR)/alct*.v))))

cp: $(FILENAMES)
	@echo $(CHIP) $(FLAV) $(FLAVOR_LX) $(FLAVOR_FULL) 
	@mkdir -p $(PRJDIR)/src
	@cp -v $(FILENAMES) $(PRJDIR)/src

clean: 
	rm -f *~
	rm -f $(VERDIR)/*.v
	rm -rf verilog_*

diff:
	@$(foreach vf, $(FNAMES), echo $(vf);$(DIFF) $(DIFFOPT) $(VERDIR)/$(vf) $(PRJDIR)/src/$(vf);)
