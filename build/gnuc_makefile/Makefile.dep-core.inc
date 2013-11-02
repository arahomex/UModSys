#---------------------------- core lib

core_lib_includes := \
  $(wildcard ../../include/umodsys/core/*.h) \
  $(wildcard ../../include/umodsys/arch/*.h) \
  $(wildcard ../../include/umodsys/platform/$(PLATFORM_ID)/*.h) \
  \
  $(wildcard ../../include/umodsys/tl/alloc/*.h) \
  $(wildcard ../../include/umodsys/tl/composite/*.h) \
  $(wildcard ../../include/umodsys/tl/math/*.h) \
  $(wildcard ../../include/umodsys/tl/string/*.h) \
  $(wildcard ../../include/umodsys/tl/util/*.h) \

core_lib_depends := \
  $(T)/umodsys.core.objects$(OSUFFIX) \
  $(T)/umodsys.core.stdtypedefs$(OSUFFIX) \
  $(T)/umodsys.core.uniqueptr$(OSUFFIX) \
  $(T)/umodsys.tl.strings$(OSUFFIX) \
  $(T)/umodsys.tl.utf$(OSUFFIX) \
  \
  $(T)/umodsys.core.syshlp_chars.$(PLATFORM_ID)$(OSUFFIX) \
  $(T)/umodsys.core.syshlp_charcode.$(PLATFORM_ID)$(OSUFFIX) \
  $(T)/umodsys.core.syshlp_files.$(PLATFORM_ID)$(OSUFFIX) \
  $(T)/umodsys.core.syshlp_proc.$(PLATFORM_ID)$(OSUFFIX) \
  $(T)/umodsys.core.syshlp_thread.$(PLATFORM_ID)$(OSUFFIX) \
  $(T)/umodsys.core.syshlp_vars.$(PLATFORM_ID)$(OSUFFIX)

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

$(T)/%$(OSUFFIX) : ../../src/core/platform/$(PLATFORM_ID)/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(core_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
