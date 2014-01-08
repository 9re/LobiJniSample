LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := lobi
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/parson \
    $(LOCAL_PATH)/../utils
LOCAL_CFLAGS := -std=c99
LOCAL_LDFLAGS := -llog -landroid
LOCAL_SRC_FILES := \
    parson/parson.c \
    jni_helper.c \
    lobi.c
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)
