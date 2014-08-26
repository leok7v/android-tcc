LOCAL_PATH := $(call my-dir)
SAVED_LOCAL_PATH := $(LOCAL_PATH)
LOCAL_ARM_NEON := true

include $(SAVED_LOCAL_PATH)/zslib/Android.mk
include $(SAVED_LOCAL_PATH)/tcc/Android.mk

APP_CFLAGS += -Wno-unused-variable
LOCAL_CFLAGS += -Wno-unused-variable
$(call import-module,android/cpufeatures)
