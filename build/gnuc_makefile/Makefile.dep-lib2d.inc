#---------------------------- core lib

lib2d_lib_includes := \
  $(wildcard ../../include/umodsys/lib/media/*.h) \

lib2d_lib_depends := \
  $(T)/umodsys.lib.2d.common$(OSUFFIX) \
  $(T)/umodsys.lib.2d.library$(OSUFFIX) \
  \
  $(T)/umodsys.mod.2d.stdlib$(OSUFFIX) \

lib2d_lib := $(T)/umodsys.lib2d$(LIBSUFFIX)

#---------------------------- make rules targets

lib2d_lib: $(lib2d_lib) 

#---------------------------- make rules static

$(lib2d_lib): $(lib2d_lib_depends)
	$(AR_ADD)$@ $^

#---------------------------- make rules generic

#---------------- lib2d lib path

$(T)/%$(OSUFFIX) : ../../src/lib/2d/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/2d/stdlib/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(lib2d_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
