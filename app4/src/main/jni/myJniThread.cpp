//
// Created by Administrator on 2017/4/24.
//
#include "com_xcc_app4_MyJniThread.h"
#include <pthread.h>
#include <android/log.h>

using namespace std;//定义命名空间，string使用

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

bool running;

void *threadStart(void *id) {
    LOGI("--这是一个线程输出的信息--");
    LOGI("--线程将持续运行--");
    while (running) {
        //sleep函数是休眠进程
//不可用 sleep(1000);
        LOGI("--线程正在持续运行--");
    }
    LOGI("--线程持续运行结束--");
    //线程结束
    pthread_exit(NULL);
}

void Java_com_xcc_app4_MyJniThread_startThread(JNIEnv * env, jclass
jc){
running = true;
pthread_t threads;
int threadid = 1;
/**
 * 创建一个线程,参数：
 * 1：返回一个不透明的，唯一的新线程标识符。
 * 2：不透明的线程属性对象。可以指定一个线程属性对象，或者NULL为缺省值。
 * 3：线程将会执行一次的C函数。
 * 4：传递的参数，不可以为空
 * 回值0是成功，其他表示错误。
 */
int rc = pthread_create(&threads, NULL, threadStart, (void *) threadid);
if(rc){
LOGI("--创建线程失败--");
}else{
LOGI("--创建线程成功--");
}
}
void Java_com_xcc_app4_MyJniThread_stopThread(JNIEnv * env, jclass
jc){
running = false;
}
