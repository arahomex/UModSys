
ifndef BUILD_MODE
BUILD_MODE=Release
endif

CXXFLAGS:=$(CXXFLAGS) -I../../include

ifeq "$(BUILD_MODE)" "Release"
CXXFLAGS:=$(CXXFLAGS) -O3
else 
CXXFLAGS:=$(CXXFLAGS) -O0
endif 

#---------------------------- directories

BINDIRA:=../../bin
BINDIR:=$(BINDIRA)/$(PLATFORM).$(BUILD_MODE)

TT:=../../tmp
T:=$(TT)/$(PLATFORM).$(BUILD_MODE)

#---------------------------- core lib

core_lib_depends := \
  $(T)/umodsys.core.objects.o \
  $(T)/umodsys.core.stdtypedefs.o \
  $(T)/umodsys.core.uniqueptr.o \

core_lib := $(T)/umodsys.core.a

#---------------------------- base lib

base_lib_depends := \
  $(T)/umodsys.base.system.o \
  $(T)/umodsys.base.console.o \
  \
  $(T)/umodsys.base.rsystem.o \
  $(T)/umodsys.base.rconsole_std.o \

base_lib := $(T)/umodsys.base.a

#---------------------------- binary

main_depends := \
  $(T)/c_main.o \
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
	-$(RM) $(T)/*.o 
	-$(RM) $(T)/*.a 
	-$(RM) $(T)/*.o.d

$(T): $(TT)
	mkdir $@
$(TT):
	mkdir $@
$(BINDIR): $(BINDIRA)
	mkdir $@
$(BINDIRA):
	mkdir $@

$(core_lib): $(core_lib_depends)
	$(AR) r $@ $^

$(base_lib): $(base_lib_depends)
	$(AR) r $@ $^

$(main): $(main_depends)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

#---------------------------- make rules generic


$(T)/%.o : ../../src/core/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
	$(CXX) -MM $(CXXFLAGS) $< -MT $@ -o $@.d

$(T)/%.o : ../../src/base/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
	$(CXX) -MM $(CXXFLAGS) $< -MT $@ -o $@.d

$(T)/%.o : ../../src/base/system/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
	$(CXX) -MM $(CXXFLAGS) $< -MT $@ -o $@.d

$(T)/%.o : ../../src/base/console/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
	$(CXX) -MM $(CXXFLAGS) $< -MT $@ -o $@.d

#---------------------------- make rules static

#---------------------------- make rules depfiles
# pull in dependency info for *existing* .o files
-include $(OBJS_ALL:.o=.o.d)

