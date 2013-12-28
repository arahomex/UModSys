#---------------------------- core lib

libui_lib_includes := \
  $(wildcard ../../include/umodsys/lib/media/*.h) \

libui_lib_depends := \
  $(T)/umodsys.lib.ui.common$(OSUFFIX) \
  \
  $(T)/umodsys.mod.ui.frames$(OSUFFIX) \
  \
  $(T)/umodsys.mod.ui.SDL_core$(OSUFFIX) \


libui_lib := $(T)/umodsys.libui$(LIBSUFFIX)

#---------------------------- make rules targets

libui_lib: $(libui_lib) 

#---------------------------- make rules static

$(libui_lib): $(libui_lib_depends)
	$(AR_ADD)$@ $^

#---------------------------- make rules generic

#---------------- libui lib path

$(T)/%$(OSUFFIX) : ../../src/lib/ui/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/ui/frames/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/ui/SDL_core/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(libui_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
