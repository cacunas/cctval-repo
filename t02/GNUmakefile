# $Id: GNUmakefile,v 1.1 1999-01-07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := main
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = /home/cristian/apps/geant4.9.6
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

ifdef ROOTSYS
CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
LDLIBS += $(shell $(ROOTSYS)/bin/root-config --libs)
endif

ifeq ($(G4SYSTEM),Linux-g++)
LDLIBS += -Wl,-rpath,$(shell $(ROOTSYS)/bin/root-config --libdir)
endif
