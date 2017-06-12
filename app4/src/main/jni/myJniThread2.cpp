//
// Created by Administrator on 2017/4/24.
//
#include "com_xcc_app4_MyJniThread2.h"
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>//usleep需要引入

using namespace std;//定义命名空间，string使用

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

int total;

void *threadStart1(void *id) {
    for (int i = 0; i < 300; i++) {
        int temp = total;
        /**
         * 在此处进行耗时操作，
         * 休眠1毫秒模拟耗时操作用时1毫秒才结束
         * 对于计算机而言，1毫秒是很长的时间了
         */
        usleep(1);//休眠1毫秒
        temp++;
        total=temp;
    }
    LOGI("--线程持运行结束--结果:%d", total);
    //线程结束
    pthread_exit(NULL);
}

void Java_com_xcc_app4_MyJniThread2_startThread(JNIEnv * env, jclass
jc){
//开3个进程演示资源抢占
total = 0;
pthread_t threads;
int id=1;
for(int i = 0;i < 3;i++){
int rc = pthread_create(&threads, NULL, threadStart1, (void *)i);
if(rc){
LOGI("--创建线程%d失败--", i);
}else{
LOGI("--创建线程%d成功--", i);
}}
}
pthread_mutex_t mutex1;//创建线程锁
void *threadStart2(void *id) {
    pthread_mutex_lock(&mutex1);//获取锁
    for (int i = 0; i < 300; i++) {
        int temp = total;
        usleep(1);//休眠1毫秒
        temp++;
        total=temp;
    }
    pthread_mutex_unlock(&mutex1);//释放锁
    LOGI("--线程持运行结束--结果:%d", total);
    //线程结束
    pthread_exit(NULL);
}
void Java_com_xcc_app4_MyJniThread2_startThread2(JNIEnv * env, jclass
jc){
total = 0;
pthread_t threads;
int id=1;
/**
 * 初始化线程锁mutex1，
 * 但实验发现，即使不初始化也能正常使用mutex1
 */
pthread_mutex_init(&mutex1,NULL);
for(int i = 0;i < 3;i++){
int rc = pthread_create(&threads, NULL, threadStart2, (void *)i);
if(rc){
LOGI("--创建线程%d失败--", i);
}else{
LOGI("--创建线程%d成功--", i);
}}
/**
 * 释放锁,
 * 实验发现，即使不释放也能正常使用。
 * 此处不能销毁锁，否则线程将无法获得到锁
 */
//pthread_mutex_destroy(&mutex1);
}
