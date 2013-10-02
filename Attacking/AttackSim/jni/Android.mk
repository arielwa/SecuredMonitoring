LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE    := attack_sim
LOCAL_SRC_FILES := attack_sim.c binder.c service_manager.c
include $(BUILD_SHARED_LIBRARY)
