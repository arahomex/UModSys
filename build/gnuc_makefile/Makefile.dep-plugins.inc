#---------------------------- main

main_depends := $(T)/umodsys.c_main$(OSUFFIX)
main_bin := umodsysc.$(PLATFORM).$(BUILD_MODE)$(BINSUFFIX)
main := $(BINDIR)/$(main_bin)
main_last := $(BINDIRL)/umodsysc


maint_depends := $(T)/umodsys.c_maint$(OSUFFIX)
maint_bin := umodsysct.$(PLATFORM).$(BUILD_MODE)$(BINSUFFIX)
maint := $(BINDIR)/$(maint_bin)
maint_last := $(BINDIRL)/umodsysct

#---------------------------- so_test1

so_test1_depends := $(T)/plugin_test1$(OSUFFIX)
so_test1 := $(BINDIR)/plugin_test1.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)

#---------------------------- so_test2

so_test2_depends := $(T)/plugin_test2$(OSUFFIX)
so_test2 := $(BINDIR)/plugin_test2.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)

#---------------------------- so_libStd

so_libStd_depends := $(T)/plugin_libStd$(OSUFFIX)
so_libStd := $(BINDIR)/plugin_libStd.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)

#---------------------------- so_libUI

so_libUI_depends := $(T)/plugin_libUI$(OSUFFIX)
so_libUI := $(BINDIR)/plugin_libUI.$(PLATFORM).$(BUILD_MODE)$(SOLIBSUFFIX)

#---------------------------- make rules targets

main: req $(main) $(maint)

so_test_s: so_test1 so_test2 so_libUI so_libStd

so_test1: req $(so_test1)
so_test2: req $(so_test2)
so_libUI: req $(so_libUI)
so_libStd: req $(so_libStd)

tests: so_test1 so_test2 main
	cd ../../data && ../bin/$(PLATFORM).$(BUILD_MODE)/$(main_bin)

#---------------------------- make rules static

$(main): $(main_depends) $(basesys_lib) $(libmedia_lib) $(libui_lib) $(lib3d_lib) $(lib2d_lib) $(base_lib) $(core_lib)
	$(CXX_LINK) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST) $(STD_LIBS)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif
	( test -f $(main_last) && rm $(main_last) ) || true
	ln -s $(main) $(main_last)

$(maint): $(maint_depends) $(basesys_lib) $(libmedia_lib) $(libui_lib) $(lib3d_lib) $(lib2d_lib) $(base_lib) $(core_lib)
	$(CXX_LINK) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST) $(STD_LIBS)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif
	( test -f $(maint_last) && rm $(maint_last) ) || true
	ln -s $(maint) $(maint_last)


$(so_test1): $(so_test1_depends) $(so_main) \
    $(libui_lib) $(lib3d_lib) $(lib2d_lib) $(libmedia_lib) \
    $(base_lib) $(core_lib)
	$(CXX_LINK_SO) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST) $(STD_LIBS)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif

$(so_test2): $(so_test2_depends) $(so_main) \
    $(libmedia_lib) $(libui_lib) $(lib3d_lib) $(lib2d_lib) \
    $(base_lib) $(core_lib)
	$(CXX_LINK_SO) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST) $(STD_LIBS)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif

$(so_libUI): $(so_libUI_depends) $(so_main) \
    $(libmedia_lib) $(libui_lib) $(lib3d_lib) $(lib2d_lib) \
    $(base_lib) $(core_lib)
	$(CXX_LINK_SO) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST) $(STD_LIBS)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif

$(so_libStd): $(so_libStd_depends) $(so_main) \
    $(libmedia_lib) $(libui_lib) $(lib3d_lib) $(lib2d_lib) \
    $(base_lib) $(core_lib)
	$(CXX_LINK_SO) $(CXXFLAGS) $(CXX_BIN)$@ $^ $(CXX_LINK_LAST) $(STD_LIBS)
ifdef CXX_LINK_POSTPROCESS
	$(CXX_LINK_POSTPROCESS) $@
endif

#---------------------------- make rules generic

$(T)/%$(OSUFFIX) : ../../src/plugin/main/%.cpp
	$(CXX_C) $(CXXFLAGS) -I../../src/basesys $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) -I../../src/basesys $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/plugin/test/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

$(T)/%$(OSUFFIX) : ../../src/plugin/libs/%.cpp
	$(CXX_C) $(CXXFLAGS) $(CXX_O)$@ $< 
ifndef NO_CXX_DEP
	$(CXX_DEP) $(CXXFLAGS) $(CXX_DEP_O)$@.d $(CXX_DEP_T)$@ $< 
endif

#---------------------------- /make rules generic

#---------------------------- make rules depfiles
# pull in dependency info for *existing* $(OSUFFIX) files
ifndef NO_CXX_DEP
-include $(main_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(maint_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(so_test1_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(so_test2_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(so_libUI_depends:$(OSUFFIX)=$(OSUFFIX).d)
-include $(so_libStd_depends:$(OSUFFIX)=$(OSUFFIX).d)
endif
