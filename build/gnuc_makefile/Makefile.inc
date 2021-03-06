include Makefile.defines.inc

#---------------------------- directories

BINDIRA:=../../bin
BINDIR:=$(BINDIRA)/$(PLATFORM).$(BUILD_MODE)
BINDIRL:=$(BINDIRA)/last

TT:=../../tmp
T:=$(TT)/$(PLATFORM).$(BUILD_MODE)

#---------------------------- make rules

.SUFFIXES=

all: target

target: req BINDIR core_lib base_lib main so_test_s

req: $(T) $(BINDIR)


clean_bin:
	-rm $(BINDIR)/*.$(PLATFORM).$(BUILD_MODE)$(BINSUFFIX)
	-rm $(BINDIR)/*.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)
	-rm $(BINDIRL)/*

clean: clean_bin
	-$(RM) $(T)/*$(OSUFFIX) 
	-$(RM) $(T)/*$(LIBSUFFIX) 
	-$(RM) $(T)/*$(OSUFFIX).d

T: $(T) 
BINDIR: $(BINDIR) $(BINDIRL)

$(T): $(TT)
	mkdir -p $@
$(TT):
	mkdir -p $@
$(BINDIR): $(BINDIRA)
	mkdir -p $@
$(BINDIRL): $(BINDIRA)
	mkdir -p $@
$(BINDIRA):
	mkdir -p $@


#---------------------------- make libraries

include Makefile.dep-core.inc
include Makefile.dep-base.inc

include Makefile.dep-libmedia.inc
include Makefile.dep-lib2d.inc
include Makefile.dep-lib3d.inc
include Makefile.dep-libui.inc

include Makefile.dep-basesys.inc

include Makefile.dep-plugins.inc

#---------------------------- make libraries
