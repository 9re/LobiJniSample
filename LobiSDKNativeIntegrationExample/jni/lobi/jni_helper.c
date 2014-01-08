#include <android/log.h>
#include <jni.h>
#include "log.h"
#include "jni_helper.h"

static JNIEnv *env_ = 0;
static jobject classloader_ = 0;
static jmethodID loadclassMethodID_ = 0;
static jobject obj_ = 0;

#ifdef __cplusplus
extern "C" {
#endif


void jnihelper_cacheEnv(JNIEnv *env) {
    env_ = env;
}

void jnihelper_init(JavaVM *vm, struct android_app *state) {
}
    //bool jnihelper_getStaticMethodInfo(JniMethodInfo *methodinfo, char *className, char *methodName, char *paramCode);
static bool jnihelper_getMethodInfoFromDefaultClassLoader(JniMethodInfo *methodInfo, const char *className, const char *methodName, const char *paramCode) {
    LOGV("jnihelper_getMethodInfo: %s::%s %s", className, methodName, paramCode);

    jclass klass =(*env_)->FindClass(env_, className);
    if (!klass) {
        LOGW("class not found: %s", className);
        return false;
    }

    jmethodID method = (*env_)->GetMethodID(env_, klass, methodName, paramCode);
    if (!method) {
        LOGW("method not found: %s %s", methodName, paramCode);
        return false;
    }

    methodInfo->env = env_;
    methodInfo->classID = (*env_)->NewGlobalRef(env_, klass);
    methodInfo->methodID = (*env_)->NewGlobalRef(env_, method);

    return true;
}

static void deleteJniMethodInfo(JniMethodInfo *methodInfo) {
    (*env_)->DeleteGlobalRef(env_, methodInfo->methodID);
    (*env_)->DeleteGlobalRef(env_, methodInfo->classID);
}

bool jnihelper_getMethodInfo(JniMethodInfo *methodInfo, const char *className, const char *methodName, const char *paramCode) {
    {
        JniMethodInfo m;
        if (!jnihelper_getMethodInfoFromDefaultClassLoader(&m, "android/app/NativeActivity", "getClassLoader", "()Ljava/lang/ClassLoader;")) {
            return false;
        }

        LOGV("jnihelper_getMethodInfo: getClassLoader ok");

        deleteJniMethodInfo(&m);
    }
 
    return true;
}



#ifdef __cplusplus
}
#endif
