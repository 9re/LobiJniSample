#ifndef JNI_HELPER_H
#define JNI_HELPER_H

#include <stdbool.h>
#include <android_native_app_glue.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct JniMethodInfo_
{
    JNIEnv *env;
    jclass classID;
    jmethodID methodID;
} JniMethodInfo;

void jnihelper_cacheEnv(JNIEnv *env);
void jnihelper_init(JavaVM *vm, struct android_app *state);
    //bool jnihelper_getStaticMethodInfo(JniMethodInfo *methodinfo, char *className, char *methodName, char *paramCode);
bool jnihelper_getMethodInfo(JniMethodInfo *methodInfo, const char *className, const char *methodName, const char *paramCode);


#ifdef __cplusplus
}
#endif

#endif
