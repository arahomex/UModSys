#---------------------------- so_test1

so_test1_depends := \
  $(T)/plugin_test1$(OSUFFIX) \
  $(so_main) $(base_lib) $(core_lib) \

so_test1 := $(BINDIR)/plugin_test1.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)

#---------------------------- make rules targets

so_test1: $(so_test1)

#---------------------------- make rules static

$(so_test1): $(so_test1_depends)
	$(CXX_LINK_SO) $(CXXFLAGS) $(CXX_BIN)$@ $^ 

#---------------------------- make rules generic

#---------------- test plugins

$(T)/%$(OSUFFIX) : ../../src/plugin/test/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic
