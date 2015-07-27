
ifdef DEBUG
BUILD_MODE=Debug
endif
ifdef RELEASE
BUILD_MODE=Release
endif
ifndef BUILD_MODE
BUILD_MODE=Release
endif

ifndef NO_CXX_DEP
ifndef CXX_DEP
CXX_DEP=$(CXX) -MM
endif
ifndef CXX_DEP_O
CXX_DEP_O=-o 
endif
ifndef CXX_DEP_T
CXX_DEP_T=-MT 
endif
endif

ifndef AR_ADD
AR_ADD=$(AR) r #
endif

ifndef BINSUFFIX
BINSUFFIX=
endif
ifndef SOLIBSUFFIX
SOLIBSUFFIX=.so
endif
ifndef LIBSUFFIX
LIBSUFFIX=.a
endif
ifndef OSUFFIX
OSUFFIX=.o
endif

ifndef CXX_C
CXX_C=$(CXX) -c 
endif
ifndef CXX_LINK_SO
CXX_LINK_SO=$(CXX) -shared -ldl -Wl,-rpath . $(CXX_LINK_FLAGS)
endif
ifndef CXX_LINK
CXX_LINK=$(CXX) -ldl -Wl,-rpath . $(CXX_LINK_FLAGS)
endif
ifndef CXX_O
CXX_O=-o 
endif
ifndef CXX_BIN
CXX_BIN=-o #
endif


ifndef CXX_RELEASE
CXX_RELEASE=-O3 -s
endif
ifndef CXX_DEBUG
CXX_DEBUG=-O0 -g
endif


CXXFLAGS += -I../../include 

ifeq "$(BUILD_MODE)" "Release"
CXXFLAGS:=$(CXXFLAGS) $(CXX_RELEASE)
else 
CXXFLAGS:=$(CXXFLAGS) $(CXX_DEBUG)
endif 

