#---------------------------- base lib

base_lib_depends := \
  $(T)/umodsys.base.system$(OSUFFIX) \
  $(T)/umodsys.base.console$(OSUFFIX) \
  $(T)/umodsys.base.module$(OSUFFIX) \
  $(T)/umodsys.base.modulecontext$(OSUFFIX) \
  $(T)/umodsys.base.moduleinfo$(OSUFFIX) \
  $(T)/umodsys.base.modulereg$(OSUFFIX) \
  \
  $(T)/umodsys.base.rsystem$(OSUFFIX) \
  $(T)/umodsys.base.rsystem_main$(OSUFFIX) \
  $(T)/umodsys.base.rmodule$(OSUFFIX) \
  $(T)/umodsys.base.rconsole_std$(OSUFFIX) \
  \
  $(T)/umodsys.base.rmodule.$(PLATFORM_ID)$(OSUFFIX) \

base_lib := $(T)/umodsys.base$(LIBSUFFIX)

#---------------------------- binary

main_depends := \
  $(T)/umodsys.c_main$(OSUFFIX) \

main := $(BINDIR)/umodsysc.$(PLATFORM).$(BUILD_MODE)$(BINSUFFIX)

#---------------------------- make rules targets

base_lib: $(base_lib) 
main: $(main)

#---------------------------- make rules static

$(base_lib): $(base_lib_depends)
	$(AR_ADD)$@ $^

$(main): $(main_depends) $(base_lib) $(core_lib)
	$(CXX_LINK) $(CXXFLAGS) $(CXX_BIN)$@ $^ 

#---------------------------- make rules generic

$(T)/%$(OSUFFIX) : ../../src/base/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/base/platform/$(PLATFORM_ID)/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/base/rsystem/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(base_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(main_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif