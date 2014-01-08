#ifndef LOBI_JNI_H
#define LOBI_JNI_H

#include <android_native_app_glue.h>
#include <jni.h>
#include <stdbool.h>

#include "_lobi_jni.h"

#ifdef __cplusplus
extern "C" {
#endif


#define LOBI_API_SUCCESS 0
#define LOBI_NETWORK_ERROR 100
#define LOBI_RESPONSE_ERROR 101
#define LOBI_FATAL_ERROR 102

typedef void (*lobi_api_callback_t)(int code, const char *response, void *userData);

bool lobi_isSignedIn(ANativeActivity *nativeActivity);
void lobi_signupWithBaseName(ANativeActivity *nativeActivity, lobi_api_callback_t callback, void *userData);
void lobi_joinGroupWithExternalID(ANativeActivity *nativeActivity, const char *groupExternalId, const char *groupName, lobi_api_callback_t callback, void *userData);
void lobi_showChatView(ANativeActivity *nativeActivity);
void lobi_showToast(ANativeActivity *nativeActivity, const char *message);

#ifdef __cplusplus
}
#endif

#endif
