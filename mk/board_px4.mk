$(info > in ardupilot/mk/board_px4.mk)

TOOLCHAIN = NATIVE
$(info including find_tools.mk)
include $(MK_DIR)/find_tools.mk
$(info including px4_targets.mk)
include $(MK_DIR)/px4_targets.mk

$(info < out ardupilot/mk/board_px4.mk)
