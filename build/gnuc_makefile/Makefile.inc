include Makefile.defines.inc

#---------------------------- directories

BINDIRA:=../../bin
BINDIR:=$(BINDIRA)/$(PLATFORM).$(BUILD_MODE)

TT:=../../tmp
T:=$(TT)/$(PLATFORM).$(BUILD_MODE)

#---------------------------- core lib

core_lib_depends := \
  $(T)/umodsys.core.objects$(OSUFFIX) \
  $(T)/umodsys.core.stdtypedefs$(OSUFFIX) \
  $(T)/umodsys.core.uniqueptr$(OSUFFIX) \

core_lib := $(T)/umodsys.core$(LIBSUFFIX)

#---------------------------- base lib

base_lib_depends := \
  $(T)/umodsys.base.system$(OSUFFIX) \
  $(T)/umodsys.base.console$(OSUFFIX) \
  \
  $(T)/umodsys.base.rsystem$(OSUFFIX) \
  $(T)/umodsys.base.rconsole_std$(OSUFFIX) \

base_lib := $(T)/umodsys.base$(LIBSUFFIX)

#---------------------------- binary

main_depends := \
  $(T)/c_main$(OSUFFIX) \
  $(base_lib) $(core_lib) \

main := $(BINDIR)/umodsysc.$(PLATFORM).$(BUILD_MODE)$(BINSUFFIX)

#---------------------------- make rules

.SUFFIXES=

all: target

target: T BINDIR core_lib base_lib main

T: $(T) 
BINDIR: $(BINDIR) 
core_lib: $(core_lib) 
base_lib: $(base_lib) 
main: $(main)

clean:
	-$(RM) $(T)/*$(OSUFFIX) 
	-$(RM) $(T)/*$(LIBSUFFIX) 
	-$(RM) $(T)/*$(OSUFFIX).d

$(T): $(TT)
	mkdir -p $@
$(TT):
	mkdir -p $@
$(BINDIR): $(BINDIRA)
	mkdir -p $@
$(BINDIRA):
	mkdir -p $@

$(core_lib): $(core_lib_depends)
	$(AR_ADD)$@ $^

$(base_lib): $(base_lib_depends)
	$(AR_ADD)$@ $^

$(main): $(main_depends)
	$(CXX) $(CXXFLAGS) $(CXX_BIN)$@ $^ 

#---------------------------- make rules generic


$(T)/%$(OSUFFIX) : ../../src/core/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/base/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/base/system/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/base/console/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- make rules static

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(OBJS_ALL:$(OSUFFIX)=$(OSUFFIX).d)
endif
