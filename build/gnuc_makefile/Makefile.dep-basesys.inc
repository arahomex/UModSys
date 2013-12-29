#---------------------------- base lib

basesys_lib_includes := 

basesys_lib_depends := \
  $(T)/umodsys.basesys.rsystem$(OSUFFIX) \
  $(T)/umodsys.basesys.rsystem_main$(OSUFFIX) \
  $(T)/umodsys.basesys.rsystem_tests$(OSUFFIX) \
  $(T)/umodsys.basesys.rmodule$(OSUFFIX) \
  $(T)/umodsys.basesys.rmodulelib$(OSUFFIX) \
  $(T)/umodsys.basesys.rmodulelibso$(OSUFFIX) \
  $(T)/umodsys.basesys.rmodulelibthis$(OSUFFIX) \
  $(T)/umodsys.basesys.rmoduleloader$(OSUFFIX) \
  $(T)/umodsys.basesys.rparameters$(OSUFFIX) \
  $(T)/umodsys.basesys.rconsole_std$(OSUFFIX) \
  \
  $(T)/umodsys.basesys.rmodule.$(PLATFORM_ID)$(OSUFFIX) \

basesys_lib := $(T)/umodsys.basesys$(LIBSUFFIX)

#---------------------------- binary

main_depends := \
  $(T)/umodsys.c_main$(OSUFFIX) \

main := $(BINDIR)/umodsysc.$(PLATFORM).$(BUILD_MODE)$(BINSUFFIX)

#---------------------------- make rules targets

basesys_lib: $(base_lib) 
main: $(main)

#---------------------------- make rules static

$(basesys_lib): $(basesys_lib_depends)
	$(AR_ADD)$@ $^

$(main): $(main_depends) $(basesys_lib) $(libmedia_lib) $(libui_lib) $(lib3d_lib) $(lib2d_lib) $(base_lib) $(core_lib)
	$(CXX_LINK) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST) $(STD_LIBS)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif

#---------------------------- make rules generic

ifndef NO_CXX_DEP

$(T)/%$(OSUFFIX) : ../../src/basesys/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
$(T)/%$(OSUFFIX) : ../../src/basesys/platform/$(PLATFORM_ID)/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 

else

$(T)/%$(OSUFFIX) : ../../src/basesys/platform/$(PLATFORM_ID)/%.cpp $(basesys_lib_includes) $(base_lib_includes) $(core_lib_includes)
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
$(T)/%$(OSUFFIX) : ../../src/basesys/%.cpp $(basesys_lib_includes) $(base_lib_includes) $(core_lib_includes)
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<

endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(basesys_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(main_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
