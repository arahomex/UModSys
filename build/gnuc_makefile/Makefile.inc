include Makefile.defines.inc

#---------------------------- directories

BINDIRA:=../../bin
BINDIR:=$(BINDIRA)/$(PLATFORM).$(BUILD_MODE)

TT:=../../tmp
T:=$(TT)/$(PLATFORM).$(BUILD_MODE)

#---------------------------- make rules

.SUFFIXES=

all: target

target: T BINDIR core_lib base_lib main so_test_s

clean:
	-$(RM) $(T)/*$(OSUFFIX) 
	-$(RM) $(T)/*$(LIBSUFFIX) 
	-$(RM) $(T)/*$(OSUFFIX).d

T: $(T) 
BINDIR: $(BINDIR) 

$(T): $(TT)
	mkdir -p $@
$(TT):
	mkdir -p $@
$(BINDIR): $(BINDIRA)
	mkdir -p $@
$(BINDIRA):
	mkdir -p $@


#---------------------------- make libraries

include Makefile.dep-core.inc
include Makefile.dep-base.inc

include Makefile.dep-libmedia.inc

include Makefile.dep-basesys.inc

include Makefile.dep-so-test.inc

