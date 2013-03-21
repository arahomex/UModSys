#---------------------------- so_test1

so_test1_depends := \
  $(T)/plugin_test1$(OSUFFIX) \

so_test1 := $(BINDIR)/plugin_test1.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)

#---------------------------- so_test2

so_test2_depends := \
  $(T)/plugin_test2$(OSUFFIX) \

so_test2 := $(BINDIR)/plugin_test2.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)

#---------------------------- make rules targets

so_test_s: so_test1 so_test2

so_test1: $(so_test1)
so_test2: $(so_test2)

#---------------------------- make rules static

$(so_test1): $(so_test1_depends) $(so_main) $(base_lib) $(core_lib)
	$(CXX_LINK_SO) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif

$(so_test2): $(so_test2_depends) $(so_main) $(base_lib) $(core_lib)
	$(CXX_LINK_SO) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif

#---------------------------- make rules generic

#---------------- test plugins

$(T)/%$(OSUFFIX) : ../../src/plugin/test/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(so_test1_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
