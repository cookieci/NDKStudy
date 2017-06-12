#include "com_xcc_app2_XccLibs.h"
#include <string.h>
#include <android/log.h>
#include <stdlib.h>

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void Java_com_xcc_app2_XccLibs_cRun(JNIEnv * env, jobject
jObj){
LOGI("--磁磁帅-->");
int *intArrays = (int *) malloc(sizeof(int) * 16);
if(NULL==intArrays){
LOGI("--申请内存失败-->");
}else{
LOGI("--申请内存成功-->");
*
intArrays = 4;
intArrays++;
*
intArrays = 2;
LOGI("--打印值-->%d", *intArrays);
intArrays--;
LOGI("--打印值-->%d", *intArrays);
LOGI("--15打印值-->%d", intArrays[15]);

LOGI("--改变申请到的内存大小-->");
intArrays = (int *) realloc(intArrays, sizeof(int) * 32);

free(intArrays);
intArrays = NULL;
}
//C++ 申请内存
int *ints = new int[16];
if(NULL==ints){
LOGI("--C++申请内存失败-->");
}else{
LOGI("--C++申请内存成功-->");
ints++;
*ints=10;
LOGI("--打印值-->%d", *ints);
ints--;
/**
 * 注:ints[1]相当于 ints+=1; *ints; ints-=1;
 */
LOGI("--打印值-->%d", ints[1]);
delete ints;
}
}