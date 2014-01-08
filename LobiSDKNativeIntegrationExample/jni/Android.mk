# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#



LOCAL_PATH := $(call my-dir)
root_LOCAL_PATH := $(LOCAL_PATH)

# lobi module
include $(LOCAL_PATH)/lobi/Android.mk

include $(CLEAR_VARS)
LOCAL_PATH := $(root_LOCAL_PATH)
LOCAL_MODULE    := native-plasma
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/utils \
    $(LOCAL_PATH)/lobi \
    $(LOCAL_PATH)/lobi/parson
LOCAL_CFLAGS := -std=c99
LOCAL_SRC_FILES := main.c
LOCAL_LDLIBS    := -lm -llog -landroid -lEGL -lGLESv1_CM
LOCAL_SHARED_LIBRARIES := liblobi
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)

