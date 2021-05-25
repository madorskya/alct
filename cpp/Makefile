# Makefile for ALCT project (test fixtures + core)
# Alexander Madorsky, University of Florida/Physics. 

include parameters.mk

#SUBDIRS = $(addsuffix .t, $(dir $(wildcard */Makefile)))
SUBDIRS = src.t tf.t

# make all targets
.PHONY: all $(MAKECMDGOALS)
all $(MAKECMDGOALS) : $(SUBDIRS)

%.t:
	echo "entering $*"
	$(MAKE) $(MAKECMDGOALS) -C $*

.PHONY:
clean:
	rm -f *~











