#---------------------------- core lib

core_lib_depends := \
  $(T)/umodsys.core.objects$(OSUFFIX) \
  $(T)/umodsys.core.stdtypedefs$(OSUFFIX) \
  $(T)/umodsys.core.uniqueptr$(OSUFFIX) \
  $(T)/umodsys.tl.strings$(OSUFFIX) \

core_lib := $(T)/umodsys.core$(LIBSUFFIX)

#---------------------------- so_main

so_main := $(T)/umodsys.so_main.$(PLATFORM_ID)$(OSUFFIX)

#---------------------------- make rules targets

core_lib: $(core_lib) 

#---------------------------- make rules static

$(core_lib): $(core_lib_depends)
	$(AR_ADD)$@ $^

#---------------------------- make rules generic

#---------------- core lib path

$(T)/%$(OSUFFIX) : ../../src/core/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------- tl lib path

$(T)/%$(OSUFFIX) : ../../src/tl/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(core_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(so_main:$(OSUFFIX)=$(OSUFFIX).d)
endif