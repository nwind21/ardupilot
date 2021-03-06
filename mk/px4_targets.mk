$(info > in mk/px4_targets.mk)

# PX4 build is via external build system

ifneq ($(PX4_ROOT),)

# cope with relative paths
ifeq ($(wildcard $(PX4_ROOT)/nuttx-configs),)
PX4_ROOT := $(shell cd $(SKETCHBOOK)/$(PX4_ROOT) && pwd)
endif

# check it is a valid PX4Firmware tree
ifeq ($(wildcard $(PX4_ROOT)/nuttx-configs),)
$(error ERROR: PX4_ROOT not set correctly - no nuttx-configs directory found)
endif

# default to PX4NuttX above the PX4Firmware tree
ifeq ($(NUTTX_SRC),)
NUTTX_SRC := $(shell cd $(PX4_ROOT)/../PX4NuttX/nuttx && pwd)/
endif

# cope with relative paths for NUTTX_SRC
ifeq ($(wildcard $(NUTTX_SRC)/configs),)
NUTTX_SRC := $(shell cd $(SKETCHBOOK)/$(NUTTX_SRC) && pwd)/
endif

ifeq ($(wildcard $(NUTTX_SRC)configs),)
$(error ERROR: NUTTX_SRC not set correctly - no configs directory found)
endif

NUTTX_GIT_VERSION := $(shell cd $(NUTTX_SRC) && git rev-parse HEAD | cut -c1-8)
PX4_GIT_VERSION   := $(shell cd $(PX4_ROOT) && git rev-parse HEAD | cut -c1-8)

EXTRAFLAGS += -DNUTTX_GIT_VERSION="\"$(NUTTX_GIT_VERSION)\""
EXTRAFLAGS += -DPX4_GIT_VERSION="\"$(PX4_GIT_VERSION)\""

# we have different config files for V1 and V2
PX4_V2_CONFIG_FILE=$(MK_DIR)/PX4/config_px4fmu-v2_APM.mk

SKETCHFLAGS=$(SKETCHLIBINCLUDES) -I$(PWD) -DARDUPILOT_BUILD -DCONFIG_HAL_BOARD=HAL_BOARD_PX4 -DSKETCHNAME="\\\"$(SKETCH)\\\"" -DSKETCH_MAIN=ArduPilot_main -DAPM_BUILD_DIRECTORY=APM_BUILD_$(SKETCH)

WARNFLAGS = -Wno-psabi -Wno-packed -Wno-error=double-promotion -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Wno-error=reorder

# avoid PX4 submodules
export GIT_SUBMODULES_ARE_EVIL = 1

PX4_MAKE = $(v) GIT_SUBMODULES_ARE_EVIL=1 make -C $(SKETCHBOOK) -f $(PX4_ROOT)/Makefile EXTRADEFINES="$(SKETCHFLAGS) $(WARNFLAGS) "'$(EXTRAFLAGS)' APM_MODULE_DIR=$(SKETCHBOOK) SKETCHBOOK=$(SKETCHBOOK) PX4_ROOT=$(PX4_ROOT) NUTTX_SRC=$(NUTTX_SRC) MAXOPTIMIZATION="-Os"
PX4_MAKE_ARCHIVES = make -C $(PX4_ROOT) NUTTX_SRC=$(NUTTX_SRC) archives MAXOPTIMIZATION="-Os"


$(info define NUTTX_SRC: $(NUTTX_SRC))
$(info define PX4_ROOT: $(PX4_ROOT))
$(info define PX4_MAKE: $(PX4_MAKE))
$(info define PX4_MAKE_ARCHIVES: $(PX4_MAKE_ARCHIVES))

.PHONY: module_mk
module_mk:
	$(info invoking px4_targets.module_mk )
	$(RULEHDR)
	$(v) echo "# Auto-generated file - do not edit" > $(SKETCHBOOK)/module.mk.new
	$(v) echo "MODULE_COMMAND = ArduPilot" >> $(SKETCHBOOK)/module.mk.new
	$(v) echo "SRCS = Build.$(SKETCH)/$(SKETCH).cpp $(SKETCHLIBSRCSRELATIVE)" >> $(SKETCHBOOK)/module.mk.new
	$(v) echo "MODULE_STACKSIZE = 4096" >> $(SKETCHBOOK)/module.mk.new
	$(v) cmp $(SKETCHBOOK)/module.mk $(SKETCHBOOK)/module.mk.new 2>/dev/null || mv $(SKETCHBOOK)/module.mk.new $(SKETCHBOOK)/module.mk
	$(v) rm -f $(SKETCHBOOK)/module.mk.new

px4: $(BUILDROOT)/make.flags $(PX4_ROOT)/Archives/px4fmu-v2.export $(SKETCHCPP) module_mk px4-io-v2
	$(info invoking px4_targets.px4 )
	$(RULEHDR)
	$(info ====== Removing PX4_ROOT config files )
	$(v) rm -f $(PX4_ROOT)/makefiles/$(PX4_V2_CONFIG_FILE)
	$(info ====== Copying PX4_v2 config file to PX4_ROOT/makefiles )
	$(v) cp $(PWD)/$(PX4_V2_CONFIG_FILE) $(PX4_ROOT)/makefiles/
	$(info ====== Invoking PX4_MAKE )
	$(PX4_MAKE) px4fmu-v2_APM
	$(v) /bin/rm -f $(SKETCH)-v2.px4
	$(v) cp $(PX4_ROOT)/Images/px4fmu-v2_APM.px4 $(SKETCH)-v2.px4
	$(v) echo "PX4 $(SKETCH) Firmware is in $(SKETCH)-v2.px4"

px4-clean:
	$(info invoking px4_targets.px4-clean )
	$(v) /bin/rm -f $(PX4_ROOT)/Images/px4fmu-v2_APM.px4
	$(v) /bin/rm -rf $(PX4_ROOT)/Archives
	$(v) /bin/rm -rf $(PX4_ROOT)/makefiles/build $(PX4_ROOT)/Build

#px4-v2-upload: px4-v2
#$(v) $(PX4_MAKE) px4fmu-v2_APM upload
#

px4-upload:
	$(info invoking px4_targets.px4-upload )
	$(RULEHDR)
	$(v) $(PX4_MAKE) px4fmu-v2_APM upload

px4-io-v2: $(PX4_ROOT)/Archives/px4io-v2.export
	$(info invoking px4_targets.px4-io-v2 )
	$(v) make -C $(PX4_ROOT) px4io-v2_default
	$(info ====== Removing px4io-v2.bin )
	$(v) /bin/rm -f px4io-v2.bin
	$(v) cp $(PX4_ROOT)/Build/px4io-v2_default.build/firmware.bin px4io-v2.bin
	$(v) cp $(PX4_ROOT)/Images/px4io-v2_default.bin px4io-v2.bin
	$(v) cp $(PX4_ROOT)/Build/px4io-v2_default.build/firmware.elf px4io-v2.elf
	$(info ====== Setting up romFS build )
	$(v) mkdir -p $(MK_DIR)/PX4/ROMFS/px4io/
	$(v) rm -f $(MK_DIR)/PX4/ROMFS/px4io/px4io.bin
	$(v) cp px4io-v2.bin $(MK_DIR)/PX4/ROMFS/px4io/px4io.bin
	$(v) mkdir -p $(MK_DIR)/PX4/ROMFS/bootloader/
	$(v) rm -f $(MK_DIR)/PX4/ROMFS/bootloader/fmu_bl.bin
	$(v) cp $(SKETCHBOOK)/mk/PX4/bootloader/px4fmuv2_bl.bin $(MK_DIR)/PX4/ROMFS/bootloader/fmu_bl.bin
	$(v) echo "PX4IOv2 Firmware is in px4io-v2.bin"

px4-io: px4-io-v2

$(PX4_ROOT)/Archives/px4fmu-v2.export:
	$(info invoking px4_targets.$(PX4_ROOT)/Archives/px4fmu-v2.export )
	$(v) $(PX4_MAKE_ARCHIVES)

$(PX4_ROOT)/Archives/px4io-v2.export:
	$(info invoking px4_targets.$(PX4_ROOT)/Archives/px4io-v2.export )
	$(v) $(PX4_MAKE_ARCHIVES)

px4-archives:
	$(info invoking px4_targets.$(PX4_ROOT)/Archives/px4-archives )
	$(v) $(PX4_MAKE_ARCHIVES)

else

px4:
	$(error ERROR: You need to add PX4_ROOT to your config.mk)

px4-clean: px4

px4-upload: px4

endif

$(info < out mk/px4_targets.mk)
