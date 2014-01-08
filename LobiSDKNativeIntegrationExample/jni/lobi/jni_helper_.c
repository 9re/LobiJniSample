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

// Only methods from android framework class are available this method
static bool getMethodInfoFromDefaultClassLoader(JniMethodInfo *methodInfo, const char *className, const char *methodName, const char *paramCode) {
    LOGV("getMethodInfoFromDefaultClassLoader: %s::%s %s", className, methodName, paramCode);
    if (!env_) {
        LOGW("JNI env not set!!");
        return false;
    }

    jclass classID = (*env_)->FindClass(env_, className);
    if (!classID) {
        LOGW("Failed to find class %s", className);
        return false;
    }
    LOGV("classID found");
    jclass gClassID = (*env_)->NewGlobalRef(env_, classID);
    LOGV("new global ref ok");

    jmethodID methodID = (*env_)->GetMethodID(env_, gClassID, methodName, paramCode);
    if (!methodID) {
        LOGW("Failed to find method %s with signature %s", methodName, paramCode);
        return false;
    }
    LOGV("method found");

    methodInfo->classID = gClassID;
    methodInfo->env = env_;
    methodInfo->methodID = (*env_)->NewGlobalRef(env_, methodID);

    LOGV("ok");

    return true;
}

static void getClassLoader(jobject nativeActivity) {
    // call Landroid/app/NativeActivity;->getClassLoader()Ljava/lang/ClassLoader;
    // from C
    //obj_  = (*env_)->NewGlobalRef(env_, nativeActivity);
    {
        JniMethodInfo m;
        if (!getMethodInfoFromDefaultClassLoader(
                &m, "android/app/NativeActivity", "getClassLoader", "()Ljava/lang/ClassLoader;")) {
            return;
        }
        LOGV("before CallObjectMethod");
        jobject classloader = (*env_)->CallObjectMethod(env_, nativeActivity, m.methodID);
    LOGV("classloader: %p", classloader);
        classloader_ = (*env_)->NewGlobalRef(env_, classloader);
     
        if (!classloader_) {
            return;
        }
        LOGV("got methodID of class loader");
    }

    {
        JniMethodInfo m;
        if (!getMethodInfoFromDefaultClassLoader(
                &m, "java/lang/ClassLoader", "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;")) {
            return;
        }
    
        loadclassMethodID_ = (*env_)->NewGlobalRef(env_, m.methodID);
        LOGV("got methodID of load class method");
    }
}

static jclass getClassID(const char *className) {
    if (!className) {
        return 0;
    }

    LOGV("class name: %s", className);
    
    jstring jstrClassName = (*env_)->NewStringUTF(env_, className);
    LOGV("new string ok");
    jclass classID = (jclass) (*env_)->CallObjectMethod(env_, classloader_, loadclassMethodID_, jstrClassName);

    LOGV("getClassID %p", classID);

    if (!classID) {
        LOGW("Unabled to find class %s", className);
        return 0;
    }

    (*env_)->DeleteLocalRef(env_, jstrClassName);

    LOGV("deleteLocal %s", className);
    return classID;
}


void jnihelper_init(JavaVM *vm, struct android_app *state) {
    LOGV("cacheJniEnv");
    (*vm)->GetEnv(vm, (void **)&env_, JNI_VERSION_1_4);

    getClassLoader(state->activity->clazz);
}

bool jnihelper_getMethodInfo(JniMethodInfo *methodInfo, const char *className, const char *methodName, const char *paramCode) {
    LOGV("getMethodInfo: %s::%s %s", className, methodName, paramCode);
    if (!env_) {
        LOGW("JNI env not set!!");
        return false;
    }

    
    jclass classID = getClassID(className);
    if (!classID) {
        return false;
    }
    LOGV("got classID!");

    return true;
}




#ifdef __cplusplus
}
#endif
