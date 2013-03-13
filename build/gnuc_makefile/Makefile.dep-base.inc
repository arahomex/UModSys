#---------------------------- base lib

base_lib_depends := \
  $(T)/umodsys.base.system$(OSUFFIX) \
  $(T)/umodsys.base.console$(OSUFFIX) \
  $(T)/umodsys.base.modulecontext$(OSUFFIX) \
  $(T)/umodsys.base.moduleinfo$(OSUFFIX) \
  \
  $(T)/umodsys.base.rsystem$(OSUFFIX) \
  $(T)/umodsys.base.rconsole_std$(OSUFFIX) \

base_lib := $(T)/umodsys.base$(LIBSUFFIX)

#---------------------------- binary

main_depends := \
  $(T)/c_main$(OSUFFIX) \
  $(base_lib) $(core_lib) \

main := $(BINDIR)/umodsysc.$(PLATFORM).$(BUILD_MODE)$(BINSUFFIX)

#---------------------------- make rules targets

base_lib: $(base_lib) 
main: $(main)

#---------------------------- make rules static

$(base_lib): $(base_lib_depends)
	$(AR_ADD)$@ $^

$(main): $(main_depends)
	$(CXX) $(CXXFLAGS) $(CXX_BIN)$@ $^ 

#---------------------------- make rules generic

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

$(T)/%$(OSUFFIX) : ../../src/base/module/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/base/memory/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic
