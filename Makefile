##############################################################################
##                                 bin2asm™                                 ##
##                                                                          ##
##                    Copyright © 2020 Alexander Nicholi                    ##
##                Released under Mozilla Public License 2.0.                ##
##############################################################################

include etc/base.mk

# name of project. used in output binary naming
PROJECT := bin2asm

# put a ‘1’ for the desired target types to compile
EXEFILE := 1
SOFILE  :=
AFILE   :=

# space-separated path list for #includes
# <system> includes
INCLUDES :=
# "local" includes
INCLUDEL := src

# space-separated library name list
LIBS      :=
LIBDIRS   :=

# ‘3P’ are in-tree 3rd-party dependencies
# 3PLIBDIR is the base directory
# 3PLIBS is the folder names in the base directory for each library
3PLIBDIR := 3rdparty
3PLIBS   := uni

# frameworks (macOS target only)
FWORKS :=

# sources
CFILES    := \
	src/main.c
HFILES    :=
CPPFILES  :=
HPPFILES  :=

# test suite sources
TES_CFILES   :=
TES_HFILES   :=
TES_CPPFILES :=
TES_HPPFILES :=

# this defines all our usual targets
include etc/targets.mk
