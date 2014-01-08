#include "lobi.h"

#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct attach_current_thread_result {
    JNIEnv *env;
    bool attached;
} attach_current_thread_result_t;

static void attach_current_thread(JavaVM *vm, attach_current_thread_result_t *res) {
    switch ((*vm)->GetEnv(vm, (void**)&res->env, JNI_VERSION_1_6)) {
    case JNI_EDETACHED:
        (*vm)->AttachCurrentThread(vm, &res->env, NULL);
        res->attached = true;
        break;
    case JNI_OK:
        res->attached = false;
        break;
    }
}

static void detach_current_thread(JavaVM *vm, attach_current_thread_result_t *res) {
    if (res->attached) {
        (*vm)->DetachCurrentThread(vm);
    }
}

bool lobi_isSignedIn(ANativeActivity *nativeActivity) {
    JavaVM *vm = nativeActivity->vm;
    attach_current_thread_result_t res;
    attach_current_thread(vm, &res);
    JNIEnv *env = res.env;
    jobject thiz = nativeActivity->clazz;

    jclass klass = (*env)->GetObjectClass(env, thiz);
    jmethodID methodj = (*env)->GetMethodID(env, klass, "isSignedIn", "()Z");

    bool signedIn = (bool)(*env)->CallBooleanMethod(env, thiz, methodj);

    detach_current_thread(vm, &res);
    
    return signedIn;
}

void lobi_signupWithBaseName(ANativeActivity *nativeActivity, lobi_api_callback_t callback, void *userData) {
    JavaVM *vm = nativeActivity->vm;
    attach_current_thread_result_t res;
    attach_current_thread(vm, &res);
    JNIEnv *env = res.env;
    jobject thiz = nativeActivity->clazz;

    jclass klass = (*env)->GetObjectClass(env, thiz);
    jmethodID methodj = (*env)->GetMethodID(env, klass, "signupWithBaseName", "(JJ)V");

    (*env)->CallVoidMethod(env, thiz, methodj, (jlong)callback, (jlong)userData);

    detach_current_thread(vm, &res);
}

void lobi_joinGroupWithExternalID(ANativeActivity *nativeActivity, const char *groupExternalId, const char *groupName, lobi_api_callback_t callback, void *userData) {
    JavaVM *vm = nativeActivity->vm;
    attach_current_thread_result_t res;
    attach_current_thread(vm, &res);
    JNIEnv *env = res.env;
    jobject thiz = nativeActivity->clazz;

    jclass klass = (*env)->GetObjectClass(env, thiz);
    jmethodID methodj = (*env)->GetMethodID(env, klass, "joinGroupWithExternalID", "(Ljava/lang/String;Ljava/lang/String;JJ)V");

    jstring jstrGroupExternalId = (*env)->NewStringUTF(env, groupExternalId);
    jstring jstrGroupName = (*env)->NewStringUTF(env, groupName);

    (*env)->CallVoidMethod(env, thiz, methodj, jstrGroupExternalId, jstrGroupName, (jlong)callback, (jlong)userData);

    (*env)->DeleteLocalRef(env, jstrGroupExternalId);
    (*env)->DeleteLocalRef(env, jstrGroupName);
    
    detach_current_thread(vm, &res);
}

void lobi_showChatView(ANativeActivity *nativeActivity) {
    JavaVM *vm = nativeActivity->vm;
    attach_current_thread_result_t res;
    attach_current_thread(vm, &res);
    JNIEnv *env = res.env;
    jobject thiz = nativeActivity->clazz;

    jclass klass = (*env)->GetObjectClass(env, thiz);
    jmethodID methodj = (*env)->GetMethodID(env, klass, "showChatView", "()V");

    (*env)->CallVoidMethod(env, thiz, methodj);

    detach_current_thread(vm, &res);
}

void lobi_showToast(ANativeActivity *nativeActivity, const char *message) {
    JavaVM *vm = nativeActivity->vm;
    attach_current_thread_result_t res;
    attach_current_thread(vm, &res);
    JNIEnv *env = res.env;
    jobject thiz = nativeActivity->clazz;

    jclass klass = (*env)->GetObjectClass(env, thiz);
    jstring jstrMessage = (*env)->NewStringUTF(env, message);
    jmethodID methodj = (*env)->GetMethodID(env, klass, "showToast", "(Ljava/lang/String;)V");
    (*env)->CallVoidMethod(env, thiz, methodj, jstrMessage);

    (*env)->DeleteLocalRef(env, jstrMessage);

    detach_current_thread(vm, &res);
}

JNIEXPORT void JNICALL Java_co_lobi_sdk_jni_CustomNativeActivity_onResult(JNIEnv *env, jobject thisObject, jint code, jstring json, jlong callback, jlong userData) {
    const char *cstrJson = (*env)->GetStringUTFChars(env, json, 0);

    if (callback) {
        lobi_api_callback_t cb = (lobi_api_callback_t) callback;
        cb(code, cstrJson, (void *)userData);
    }

    (*env)->ReleaseStringUTFChars(env, json, cstrJson);
}

#ifdef __cplusplus
}
#endif
