#include "com_xcc_ndkstudy_GetString.h"
#include <string.h>
#include <android/log.h>

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

jstring Java_com_xcc_ndkstudy_GetString_stringToJni(JNIEnv *jniEnv, jobject jobj) {
    LOGI("test");
    jstring jStr = jniEnv->NewStringUTF("磁磁帅");
    jboolean isCopy;
    const char *str = jniEnv->GetStringUTFChars(jStr, &isCopy);
    if (0 != str) {
        LOGI("test%s", str);
    }
    jniEnv->ReleaseStringUTFChars(jStr, str);
    //delete str;
    LOGI("test%s", str);
    str = 0;
    return jStr;
}