include Makefile.defines.inc

#---------------------------- directories

BINDIRA:=../../bin
BINDIR:=$(BINDIRA)/$(PLATFORM).$(BUILD_MODE)

TT:=../../tmp
T:=$(TT)/$(PLATFORM).$(BUILD_MODE)

#---------------------------- make rules

.SUFFIXES=

all: target

target: T BINDIR core_lib base_lib main

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

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(OBJS_ALL:$(OSUFFIX)=$(OSUFFIX).d)
endif
