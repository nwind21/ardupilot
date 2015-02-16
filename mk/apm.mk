$(info > in mk/apm.mk)

# find the mk/ directory, which is where this makefile fragment
# lives. (patsubst strips the trailing slash.)
SYSTYPE			:=	$(shell uname)


MK_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
GIT_VERSION := $(shell git rev-parse HEAD | cut -c1-8)
EXTRAFLAGS += -DGIT_VERSION="\"$(GIT_VERSION)\""

$(info define SYSTYPE: $(SYSTYPE))
$(info define MK_DIR: $(MK_DIR))

# find key paths and system type

# Save the system type for later use.
#


# force LANG to C so awk works sanely on MacOS
export LANG=C

#
# Locate the sketch sources based on the initial Makefile's path
#
SRCROOT			:=	$(realpath $(dir $(firstword $(MAKEFILE_LIST))))


#
# We need to know the location of the sketchbook.  If it hasn't been overridden,
# try the parent of the current directory.  If there is no libraries directory
# there, assume that we are in a library's examples directory and try backing up
# further.
#
ifeq ($(SKETCHBOOK),)
  SKETCHBOOK		:=	$(shell cd $(SRCROOT)/.. && pwd)
  ifeq ($(wildcard $(SKETCHBOOK)/libraries),)
    SKETCHBOOK		:=	$(shell cd $(SRCROOT)/../.. && pwd)
  endif
  ifeq ($(wildcard $(SKETCHBOOK)/libraries),)
    SKETCHBOOK		:=	$(shell cd $(SRCROOT)/../../.. && pwd)
  endif
  ifeq ($(wildcard $(SKETCHBOOK)/libraries),)
    SKETCHBOOK		:=	$(shell cd $(SRCROOT)/../../../.. && pwd)
  endif
  ifeq ($(wildcard $(SKETCHBOOK)/libraries),)
    $(error ERROR: cannot determine sketchbook location - please specify on the commandline with SKETCHBOOK=<path>)
  endif
else
  ifeq ($(wildcard $(SKETCHBOOK)/libraries),)
    $(warning WARNING: sketchbook directory $(SKETCHBOOK) contains no libraries)
  endif
endif

#
# Work out the sketch name from the name of the source directory.
#
SKETCH			:=	$(lastword $(subst /, ,$(SRCROOT)))
# Workaround a $(lastword ) bug on cygwin
ifeq ($(SKETCH),)
  WORDLIST		:=	$(subst /, ,$(SRCROOT))
  SKETCH		:=	$(word $(words $(WORDLIST)),$(WORDLIST))
endif

#
# Work out where we are going to be building things
#
TMPDIR			?=	/tmp

ifneq ($(findstring px4, $(MAKECMDGOALS)),)
# when building px4 we need all sources to be inside the sketchbook directory
# as the NuttX build system relies on it
BUILDROOT		:=	$(SKETCHBOOK)/Build.$(SKETCH)
endif

ifeq ($(BUILDROOT),)
BUILDROOT		:=	$(abspath $(TMPDIR)/$(SKETCH).build)
endif

# Jump over the next makefile sections when runing a "make configure"
ifneq ($(MAKECMDGOALS),configure)

################################################################################
# Config options
#
# The Makefile calling us must specify BOARD
#
$(info including $(SKETCHBOOK)/config.mk)
include $(SKETCHBOOK)/config.mk

ifeq ($(PORT),)
$(error ERROR: could not locate $(SKETCHBOOK)/config.mk, please run 'make configure' first and edit config.mk)
endif

ifneq ($(APPDIR),)
# this is a recusive PX4 build
HAL_BOARD = HAL_BOARD_PX4
endif

# handle target based overrides for board type
ifneq ($(findstring px4, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_PX4
endif

ifneq ($(findstring sitl, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_AVR_SITL
HAL_BOARD_SUBTYPE = HAL_BOARD_SUBTYPE_NONE
endif

ifneq ($(findstring linux, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_LINUX
HAL_BOARD_SUBTYPE = HAL_BOARD_SUBTYPE_LINUX_NONE
endif

ifneq ($(findstring erle, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_LINUX
HAL_BOARD_SUBTYPE = HAL_BOARD_SUBTYPE_LINUX_ERLE
endif

ifneq ($(findstring pxf, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_LINUX
HAL_BOARD_SUBTYPE = HAL_BOARD_SUBTYPE_LINUX_PXF
endif

ifneq ($(findstring navio, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_LINUX
HAL_BOARD_SUBTYPE = HAL_BOARD_SUBTYPE_LINUX_NAVIO
endif


ifneq ($(findstring apm1, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_APM1
HAL_BOARD_SUBTYPE = HAL_BOARD_SUBTYPE_AVR_APM1
endif

ifneq ($(findstring apm2, $(MAKECMDGOALS)),)
HAL_BOARD = HAL_BOARD_APM2
HAL_BOARD_SUBTYPE = HAL_BOARD_SUBTYPE_AVR_APM2
endif

# default to PX4
ifeq ($(HAL_BOARD),)
#$(warning No HAL_BOARD in config.mk - defaulting to HAL_BOARD_APM2)
$(info Defaulting to PX4 build)
HAL_BOARD = HAL_BOARD_PX4
endif

HARDWARE		?=	arduino
ifeq ($(BOARD),)
BOARD = mega2560
endif

ifneq ($(findstring apm1-1280, $(MAKECMDGOALS)),)
BOARD = mega
endif

$(info define SRCROOT: $(SRCROOT) )
$(info define SKETCH: $(SKETCH) )
$(info define SKETCHBOOK: $(SKETCHBOOK) )
$(info define BOARD: $(BOARD) )
$(info define HAL_BOARD: $(HAL_BOARD) )
$(info define HAL_BOARD_SUBTYPE: $(HAL_BOARD_SUBTYPE) )

endif

# short-circuit build for the configure target
ifeq ($(MAKECMDGOALS),configure)
include $(MK_DIR)/configure.mk

else

$(info including targets.mk)
# common makefile components
include $(MK_DIR)/targets.mk

$(info including sketch_sources.mk)
include $(MK_DIR)/sketch_sources.mk

# ifneq ($(MAKECMDGOALS),clean)

# board specific includes
ifeq ($(HAL_BOARD),HAL_BOARD_APM1)
$(info including $(MK_DIR)/board_avr.mk)
include $(MK_DIR)/board_avr.mk
endif

ifeq ($(HAL_BOARD),HAL_BOARD_APM2)
$(info including $(MK_DIR)/board_avr.mk)
include $(MK_DIR)/board_avr.mk
endif

ifeq ($(HAL_BOARD),HAL_BOARD_AVR_SITL)
$(info including board_px4.mk)
include $(MK_DIR)/board_native.mk
endif

ifeq ($(HAL_BOARD),HAL_BOARD_LINUX)
$(info including board_linux.mk)
include $(MK_DIR)/board_linux.mk
endif

ifeq ($(HAL_BOARD),HAL_BOARD_PX4)
$(info including board_px4.mk)
include $(MK_DIR)/board_px4.mk
endif

# endif

endif

$(info < out mk/apm.mk)
