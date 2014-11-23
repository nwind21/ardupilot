# find the mk/ directory, which is where this makefile fragment
# lives. (patsubst strips the trailing slash.)
SYSTYPE			:=	$(shell uname)

ifneq ($(findstring CYGWIN, $(SYSTYPE)),) 
  MK_DIR := $(shell cygpath -m ../mk)
else
  MK_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
endif

$(info including environ.mk)
include $(MK_DIR)/environ.mk

# short-circuit build for the configure target
ifeq ($(MAKECMDGOALS),configure)
include $(MK_DIR)/configure.mk

else

$(info including targets.mk)
# common makefile components
include $(MK_DIR)/targets.mk
$(info including sketch_sources.mk)
include $(MK_DIR)/sketch_sources.mk

ifneq ($(MAKECMDGOALS),clean)
# board specific includes
ifeq ($(HAL_BOARD),HAL_BOARD_APM1)
$(info including board_avr.mk for apm1)
include $(MK_DIR)/board_avr.mk
endif

ifeq ($(HAL_BOARD),HAL_BOARD_APM2)
$(info including board_avr.mk for apm2)
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

ifeq ($(HAL_BOARD),HAL_BOARD_VRBRAIN)
include $(MK_DIR)/board_vrbrain.mk
endif

ifeq ($(HAL_BOARD),HAL_BOARD_FLYMAPLE)
include $(MK_DIR)/board_flymaple.mk
endif

endif

endif
