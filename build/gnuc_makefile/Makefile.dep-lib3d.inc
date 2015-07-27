#---------------------------- core lib

lib3d_lib_includes := \
  $(wildcard ../../include/umodsys/lib/3d/*.h) \

lib3d_lib_depends := \
  $(T)/umodsys.lib.3d.common$(OSUFFIX) \
  \
  $(T)/umodsys.mod.3d.stdlib$(OSUFFIX) \

lib3d_lib := $(T)/umodsys.lib3d$(LIBSUFFIX)

#---------------------------- make rules targets

lib3d_lib: $(lib3d_lib) 

#---------------------------- make rules static

$(lib3d_lib): $(lib3d_lib_depends)
	$(AR_ADD)$@ $^

#---------------------------- make rules generic

#---------------- lib3d lib path

$(T)/%$(OSUFFIX) : ../../src/lib/3d/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/3d/stdlib/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(lib3d_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
