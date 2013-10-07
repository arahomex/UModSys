#---------------------------- base lib

base_lib_includes := \
  $(wildcard ../../include/umodsys/base/*.h)

base_lib_depends := \
  $(T)/umodsys.base.debug$(OSUFFIX) \
  $(T)/umodsys.base.console$(OSUFFIX) \
  $(T)/umodsys.base.system$(OSUFFIX) \
  $(T)/umodsys.base.module$(OSUFFIX) \
  $(T)/umodsys.base.moduleloader$(OSUFFIX) \
  $(T)/umodsys.base.modulecontext$(OSUFFIX) \
  $(T)/umodsys.base.moduleinfo$(OSUFFIX) \
  $(T)/umodsys.base.modulereg$(OSUFFIX) \
  $(T)/umodsys.base.moduleregchain$(OSUFFIX) \
  \
  $(T)/umodsys.so_main.$(PLATFORM_ID)$(OSUFFIX) \

base_lib := $(T)/umodsys.base$(LIBSUFFIX)

#---------------------------- make rules targets

base_lib: $(base_lib)

#---------------------------- make rules static

$(base_lib): $(base_lib_depends)
	$(AR_ADD)$@ $^

#---------------------------- make rules generic

ifndef NO_CXX_DEP

$(T)/%$(OSUFFIX) : ../../src/base/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
$(T)/%$(OSUFFIX) : ../../src/base/platform/$(PLATFORM_ID)/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 

else

$(T)/%$(OSUFFIX) : ../../src/base/platform/$(PLATFORM_ID)/%.cpp $(base_lib_includes) $(core_lib_includes)
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
$(T)/%$(OSUFFIX) : ../../src/base/%.cpp $(base_lib_includes) $(core_lib_includes)
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<

endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(base_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
