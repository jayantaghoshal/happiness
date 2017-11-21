#include <stdio.h>
#include <vcc/localconfig.h>
#include "JNIHelp.h"
#include "jni.h"
#include "utils/Log.h"

#define LOG_TAG "LocalConfigJNICLIB"
auto *lcfg = vcc::LocalConfigDefault();

std::string jStringToString(JNIEnv *env, jstring jStr) {
    if (!jStr) return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

jstring stringToJString(JNIEnv *env, const std::string &nativeString) {
    return env->NewStringUTF(nativeString.c_str());
}

static jint getInt(JNIEnv *env, jobject jObject, jstring javaString) {
    ALOGV("+ getInt");
    (void)jObject;
    int value = 0;

    try {
        value = lcfg->GetInt(jStringToString(env, javaString));
    } catch (const std::exception &e) {
        jniThrowRuntimeException(env, e.what());
    }

    ALOGV("- getInt");
    return value;
}

static jstring getString(JNIEnv *env, jobject jObject, jstring javaString) {
    ALOGV("+ getString");
    (void)jObject;

    std::string value = "";

    try {
        value = lcfg->GetString(jStringToString(env, javaString));
    } catch (const std::exception &e) {
        jniThrowRuntimeException(env, e.what());
    }

    ALOGV("- getString");
    return stringToJString(env, value);
}

// TODO: Make it sure if it is needed, if yes then fix issue with boolean default and make a generic API function or
// remove it.
static jboolean getBool(JNIEnv *env, jobject jObject, jstring javaString) {
    ALOGV("+ getBool");
    (void)jObject;

    bool value = false;
    try {
        value = lcfg->GetBool(jStringToString(env, javaString));
    } catch (const std::exception &e) {
        jniThrowRuntimeException(env, e.what());
    }

    ALOGV("- getBool");
    return value;
}

static jdouble getDouble(JNIEnv *env, jobject jObject, jstring javaString) {
    ALOGV("+ getDouble");
    (void)jObject;

    double value = -1;

    try {
        value = lcfg->GetDouble(jStringToString(env, javaString));
    } catch (const std::exception &e) {
        jniThrowRuntimeException(env, e.what());
    }

    ALOGV("- getDouble");
    return value;
}

/*
//TODO: Make it sure if it is needed, if yes then fix issue with boolean default and make a generic API function or
remove it.
//TODO: Fix with proper return type i.e. StringArray type
//TODO: Corresponding jira ticket: https://flow.jira.cm.volvocars.biz/browse/PSS370-8668
static jint getStringArray(JNIEnv *env, jobject jObject, jstring javaString)
{
    (void)jObject;
    printf("+ LcfLibrary::getInt: %s \n","something");
    auto * lcfg = vcc::LocalConfigDefault();
    int value = lcfg->GetInt(jStringToString(env,javaString));

    return value;
}
*/

static const JNINativeMethod sMethods[] = {
        {"getNativeInt", "(Ljava/lang/String;)I", (void *)getInt},
        {"getNativeString", "(Ljava/lang/String;)Ljava/lang/String;", (void *)getString},
        {"getNativeBool", "(Ljava/lang/String;)Z", (void *)getBool},
        {"getNativeDouble", "(Ljava/lang/String;)D", (void *)getDouble},

        // TODO: based on above TODO comment, take a corresponding action.
        //{ "getNativeStringArray", "(Ljava/lang/String;)[Ljava/lang/String;", (void*) getStringArray },
};

int register_android_LocalConfig(JNIEnv *env) {
    return jniRegisterNativeMethods(env, "com/volvocars/localconfig/LcfLibrary", sMethods, NELEM(sMethods));
}

extern "C" jint JNI_OnLoad(JavaVM *vm, void * /* reserved */) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("GetEnv failed!");
        return result;
    }
    ALOG_ASSERT(env, "Could not retrieve the env!");

    register_android_LocalConfig(env);

    return JNI_VERSION_1_4;
}
