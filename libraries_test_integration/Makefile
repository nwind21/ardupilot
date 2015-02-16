# find the mk/ directory, which is where this makefile fragment
# lives. (patsubst strips the trailing slash.)
SYSTYPE			:=	$(shell uname)
MK_DIR := ../mk

# common makefile components
default: all


.PHONY: etags
etags:
	cd .. && etags -f ArduCopter/TAGS --lang=c++ $$(git ls-files ArduCopter libraries)
	cd .. && etags -f ArduPlane/TAGS --lang=c++ $$(git ls-files ArduPlane libraries)

#include $(MK_DIR)/sketch_sources.mk

TOOLCHAIN = NATIVE

include $(MK_DIR)/find_tools.mk

# Library object files
BUILDROOT       :=  /tmp/build.libraries_gtest
LIBRARIES_RPATH := ../libraries
SRCSUFFIXES     := *.cpp *.c *.S

#
# Tool options
#
HAL_BOARD       =   HAL_BOARD_AVR_SITL
TOOLCHAIN       =   NATIVE
DEFINES         =   -DF_CPU=$(F_CPU)
DEFINES        +=   $(EXTRAFLAGS) # from user config.mk
DEFINES        +=   -DCONFIG_HAL_BOARD=$(HAL_BOARD) -DSKETCH="\"GTEST\""
WARNFLAGS       =   -Wformat -Wall -Wshadow -Wpointer-arith -Wcast-align -Wno-unused-parameter -Wno-missing-field-initializers
WARNFLAGS      +=   -Wwrite-strings -Wformat=2
WARNFLAGSCXX    =   -Wno-reorder
DEPFLAGS        =   -MD -MT

#CXXOPTS         =   -ffunction-sections -fdata-sections -fno-exceptions -fsigned-char
CXXOPTS         =   -fno-exceptions -fsigned-char
COPTS           =   -ffunction-sections -fdata-sections -fsigned-char

ASOPTS          =   -x assembler-with-cpp

CPUFLAGS     = -D_GNU_SOURCE
CPULDFLAGS   = -g
OPTFLAGS     ?= -O0 -g

CXXFLAGS        =   -g -fprofile-arcs -ftest-coverage $(CPUFLAGS) $(DEFINES) $(LISTOPTS) $(OPTFLAGS)
CXXFLAGS       +=   $(WARNFLAGS) $(WARNFLAGSCXX) $(DEPFLAGS) $(CXXOPTS)
CFLAGS          =   -g -fprofile-arcs -ftest-coverage $(CPUFLAGS) $(DEFINES) $(LISTOPTS) $(OPTFLAGS)
CFLAGS         +=   $(WARNFLAGS) $(DEPFLAGS) $(COPTS)
ASFLAGS         =   -g $(CPUFLAGS) $(DEFINES) $(LISTOPTS) $(DEPFLAGS)
ASFLAGS        +=   $(ASOPTS)
LDFLAGS         =   -g -fprofile-arcs $(CPUFLAGS) $(OPTFLAGS) $(WARNFLAGS) -Wl,--gc-sections -Wl,-Map,$(MAPFILE)
LISTOPTS        =   -Wa,-adhlns=$(1:.o=.lst)

ifeq ($(VERBOSE),)
v = @
else
v =
endif



################################################################################
# Application Make
#
APPLICATION_SOURCE_FOLDERS :=\
    .\
    ./AP_InertialSensor\
    ./AP_InertialSensor/HIL\
    ./AP_Math\
    ./Harness\
    ./Harness/AP_InertialSensor_Mock

LIBRARY_SOURCE_FOLDERS :=\
    $(LIBRARIES_RPATH)/AP_InertialSensor\
    $(LIBRARIES_RPATH)/AP_Baro\
    $(LIBRARIES_RPATH)/AP_BattMonitor\
    $(LIBRARIES_RPATH)/AP_Compass\
    $(LIBRARIES_RPATH)/AP_GPS\
    $(LIBRARIES_RPATH)/AP_HAL\
    $(LIBRARIES_RPATH)/AP_HAL/utility\
    $(LIBRARIES_RPATH)/AP_Param\
    $(LIBRARIES_RPATH)/AP_Math\
    $(LIBRARIES_RPATH)/AP_Mission\
    $(LIBRARIES_RPATH)/AP_NavEKF\
    $(LIBRARIES_RPATH)/AP_Declination\
    $(LIBRARIES_RPATH)/DataFlash\
    $(LIBRARIES_RPATH)/Filter\
    $(LIBRARIES_RPATH)/SITL\
    $(LIBRARIES_RPATH)/StorageManager\
    $(LIBRARIES_RPATH)/GCS_MAVLink\
    $(LIBRARIES_RPATH)/AP_Notify

LIBRARY_EXPLICIT_SOURCE :=\


INCLUDE_FOLDERS :=\
    /home/vagrant/gmock-1.7.0/include\
    /home/vagrant/gmock-1.7.0/gtest/include\
    ./Harness\
    ./Harness/AP_HAL_Mock\
    ./Harness/AP_InertialSensor_Mock\
    $(LIBRARIES_RPATH)/AP_ADC\
    $(LIBRARIES_RPATH)/AP_ADC_AnalogSource\
    $(LIBRARIES_RPATH)/AP_AHRS\
    $(LIBRARIES_RPATH)/AP_Airspeed\
    $(LIBRARIES_RPATH)/AP_BattMonitor\
    $(LIBRARIES_RPATH)/AP_Compass\
    $(LIBRARIES_RPATH)/AP_Common\
    $(LIBRARIES_RPATH)/AP_GPS\
    $(LIBRARIES_RPATH)/AP_HAL\
    $(LIBRARIES_RPATH)/AP_Math\
    $(LIBRARIES_RPATH)/AP_NavEKF\
    $(LIBRARIES_RPATH)/AP_Progmem\
    $(LIBRARIES_RPATH)/AP_Rally\
    $(LIBRARIES_RPATH)/AP_Terrain\
    $(LIBRARIES_RPATH)/AP_Vehicle\
    $(LIBRARIES_RPATH)/DataFlash\
    $(LIBRARIES_RPATH)/Filter

LIBRARY_FOLDERS    := \
    /home/vagrant/gmock-1.7.0/lib/.libs\
    /home/vagrant/gmock-1.7.0/gtest/lib/.libs\
    mavlink/include

LIBRARIES := \
    m\
    pthread\
    zmq\
    gtest\
    gmock

APPLICATION_SOURCE   = $(foreach folder, $(APPLICATION_SOURCE_FOLDERS), $(wildcard $(folder)/*.cpp))
LIBRARY_SOURCE       = $(foreach folder, $(LIBRARY_SOURCE_FOLDERS), $(wildcard $(folder)/*.cpp))
LIBRARY_HEADER       = $(foreach folder, $(LIBRARY_SOURCE_FOLDERS), $(wildcard $(folder)/*.h))
LIBRARY_SOURCE      += $(LIBRARY_EXPLICIT_SOURCE)
APPLICATION_OBJECTS  = $(subst ./,,$(foreach source_file, $(APPLICATION_SOURCE), $(addsuffix .o,$(BUILDROOT)/$(dir $(source_file))$(notdir $(basename $(source_file))))))
LIBRARY_OBJECTS      = $(subst ../,,$(foreach source_file, $(LIBRARY_SOURCE), $(addsuffix .o,$(BUILDROOT)/$(dir $(source_file))$(notdir $(basename $(source_file))))))
CXX_INCLUDE_PATHS   := $(foreach folder, $(INCLUDE_FOLDERS) $(APPLICATION_SOURCE_FOLDERS), $(addprefix -I, $(folder)))
CXX_INCLUDE_PATHS   += $(foreach folder, $(INCLUDE_FOLDERS) $(LIBRARY_SOURCE_FOLDERS), $(addprefix -I, $(folder)))
CXX_LIBRARIES       := $(foreach library, $(LIBRARY_FOLDERS), $(addprefix -L, $(library)))
CXX_LIBRARIES       += $(foreach library, $(LIBRARIES), $(addprefix -l, $(library)))


################################################################################
# Built products
#

# The ELF file
APPLICATION        = $(BUILDROOT)/libraries_gtest.elf
MAPFILE            = $(BUILDROOT)/libraries_gtest.map

################################################################################
# Targets
#

all: $(APPLICATION)

run:
	$(info $(OPTION))
	$(APPLICATION) $(OPTION)

# print-%:
# echo "$*=$($*)"

################################################################################
# Rules
#

PREREQUISITES:
	@echo "========================================================================================"
	@echo "=== BUILDING ==="
	@echo "$(APPLICATION) IN $(BUILDROOT)"
	@echo "=== CXXFLAGS ==="
	@echo "$(CXXFLAGS)"
	@echo "=== LDFLAGS ==="
	@echo "$(LDFLAGS)"
	@echo "=== CXX_INCLUDE_PATHS ==="
	@echo "$(CXX_INCLUDE_PATHS)"
	@echo "=== CXX_LIBRARIES ==="
	@echo "$(CXX_LIBRARIES)"
	@echo "========================================================================================"

################
# Link the final object
$(APPLICATION): PREREQUISITES $(APPLICATION_OBJECTS) $(LIBRARY_OBJECTS)
	$(info === LINKING... $(APPLICATION) ===)
	$(v)$(LD) $(LDFLAGS) $(APPLICATION_OBJECTS) $(LIBRARY_OBJECTS) -o $@ $(CXX_LIBRARIES)

clean:
	@rm -fr $(APPLICATION_OBJECTS) $(LIBRARY_OBJECTS)

################
# Define the generic object to source dependency build rule
define BUILD_RULE
$(1): $(2)$(patsubst %.o,%.cpp,$(subst $(BUILDROOT),,$(1)))
	@echo $(1)
	@mkdir -p $(dir $(1))
	$(v)$(CXX) $(CXXFLAGS) $(CXX_INCLUDE_PATHS) -c $$< -o $$@
endef

$(foreach objects,$(APPLICATION_OBJECTS),$(eval $(call BUILD_RULE,$(objects),.)))
$(foreach objects,$(LIBRARY_OBJECTS),$(eval $(call BUILD_RULE,$(objects),..)))
