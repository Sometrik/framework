# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# C compiler 
CC = gcc

# C++ compiler 
CXX = g++

# Standard flags for CC 
CFLAGS ?= 

INCLUDE_FLAGS ?= -I../../canvas/include -I../include -I../../httpclient/include -I./include
# Standard flags for C++
# -DGL_ES
CXXFLAGS ?= -Wall -DGLM_FORCE_RADIANS `pkg-config --cflags gtk+-3.0`
CXXFLAGS+= $(INCLUDE_FLAGS) -O2 --std=c++11
CXXFLAGS+= -Werror=write-strings -Werror=return-type -Werror=conversion-null -Werror=parentheses -Werror=switch -Werror=address -Werror=trigraphs -Werror=non-virtual-dtor -Werror=return-local-addr -Werror=multichar -Werror=enum-compare
CXXFLAGS+= -Wuninitialized -Wpointer-arith -fno-diagnostics-show-caret -Wno-sign-conversion -Wno-float-conversion -D__LINUX__
# -Wshadow 

# Standard linker flags 
LDFLAGS ?= -lcurl -ldl -lcrypto -lpng -lcairo -lpthread -lz -lX11 `pkg-config --libs gtk+-3.0`

# Profiling
CXXFLAGS+=-pg
LDFLAGS+=-pg

### Variables: ###

CPPDEPS = -MT$@ -MF`echo $@ | sed -e 's,\.o$$,.d,'` -MD -MP

OBJECTS = \
	Example1.o \
	../src/DateTime.o \
	../src/StringUtils.o \
	../src/EventHandler.o \
	../src/Element.o \
	../src/Selection.o \
	../src/FWApplication.o \
	../src/Runnable.o \
	../src/PosixThread.o \
	../src/gtk/PlatformGtk.o \
	../../canvas/src/Color.o \
	../../canvas/src/ContextCairo.o \
	../../canvas/src/Path2D.o \
	../../canvas/src/Image.o \
	../../canvas/src/ImageData.o \
	../../canvas/src/PackedImageData.o \
	../../canvas/src/rg_etc1.o \
	../../canvas/src/dxt.o \
	../../canvas/src/FloydSteinberg.o \
	../../httpclient/src/URI.o \
	../../httpclient/src/FNV.o \
	../../httpclient/src/OAuth.o \
	../../httpclient/src/Base64.o \
	../../httpclient/src/SHA1.o \
	../../httpclient/src/CurlClient.o

### Targets: ###

all: example1

install: 

uninstall: 

clean: 
	rm -f $(OBJECTS)

example1: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o :
	$(CXX) -c $(CXXFLAGS) $< -o $@

.c.o :
	gcc -c $< -o $@

.PHONY: all install uninstall clean
