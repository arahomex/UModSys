
ifndef BUILD_MODE
BUILD_MODE=Release
endif

CXXFLAGS:=$(CXXFLAGS) -I../../include

ifeq "$(BUILD_MODE)" "Release"
CXXFLAGS:=$(CXXFLAGS) -O3
else 
CXXFLAGS:=$(CXXFLAGS) -O0
endif 

TT:=../../tmp
T:=$(TT)/$(PLATFORM).$(BUILD_MODE)

core_lib_depends := \
  $(T)/umodsys.core.objects.o \
  $(T)/umodsys.core.stdtypedefs.o \
  $(T)/umodsys.core.uniqueptr.o \

core_lib := $(T)/umodsys.core.a

#---------------------------- make rules

.SUFFIXES=

target: $(T) $(core_lib)

clean:
	-$(RM) $(T)/*.o 
	-$(RM) $(T)/*.a 
	-$(RM) $(T)/*.o.d

$(T): $(TT)
	mkdir $@

$(TT):
	mkdir $@

$(core_lib): $(core_lib_depends)
	$(AR) r $(core_lib) $(core_lib_depends)


#---------------------------- make rules generic


$(T)/%.o : ../../src/core/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
	$(CXX) -MM $(CXXFLAGS) $< -MT $@ -o $@.d


#---------------------------- make rules static

#---------------------------- make rules depfiles
# pull in dependency info for *existing* .o files
-include $(OBJS_ALL:.o=.o.d)

