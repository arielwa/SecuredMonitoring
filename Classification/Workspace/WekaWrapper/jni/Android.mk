LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_LDLIBS    := -llog
LOCAL_MODULE    := weka_jni
LOCAL_SRC_FILES := weka_jni.c
include $(BUILD_SHARED_LIBRARY)
