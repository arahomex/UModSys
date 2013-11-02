#---------------------------- core lib

libmedia_lib_includes := \
  $(wildcard ../../include/umodsys/lib/media/*.h) \

libmedia_lib_depends := \
  $(T)/umodsys.lib.media.common$(OSUFFIX) \
  \
  $(T)/umodsys.mod.media.stdio$(OSUFFIX) \
  \
  $(T)/umodsys.mod.media.std$(OSUFFIX) \
  \
  $(T)/umodsys.mod.media.images_std$(OSUFFIX) \

libmedia_lib := $(T)/umodsys.libmedia$(LIBSUFFIX)

#---------------------------- make rules targets

libmedia_lib: $(libmedia_lib) 

#---------------------------- make rules static

$(libmedia_lib): $(libmedia_lib_depends)
	$(AR_ADD)$@ $^

#---------------------------- make rules generic

#---------------- libmedia lib path

$(T)/%$(OSUFFIX) : ../../src/lib/media/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/media/libmedia/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/media/stdio/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/media/std/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/module/media/images_std/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $<
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(libmedia_lib_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
