#include "com_xcc_app3_MyJni2.h"
#include <string>
#include <android/log.h>
#include <stdio.h>

using namespace std;

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void Java_com_xcc_app3_MyJni2_fileW(JNIEnv * env, jclass
jc){//写入
FILE *file=fopen("/data/data/com.xcc.app3/jniRW2.txt","w+");

if(NULL==file){
LOGI("--文件打开失败--");
}else{
LOGI("--文件打开成功--");
string str="--这是磁磁帅写入的文件数据--";
//string str="--this is cicishuai write file data--";

LOGI("EOF:%d",EOF);//实验得出，EOP为-1；

if(EOF==fputs(str.data(),file)){
LOGI("--文件写入出错--");
}else LOGI("--文件写入成功--");

if(0!=fclose(file)){
LOGI("--文件关闭失败--");
}else LOGI("--文件关闭成功--");
delete file;
}
}

jstring Java_com_xcc_app3_MyJni2_fileR(JNIEnv * env, jclass
jc){//读取
FILE *file=fopen("/data/data/com.xcc.app3/jniRW2.txt","r+");
if(NULL==file){
LOGI("--文件打开失败--");
}else{
LOGI("--文件打开成功--");
int len=10;
char p[len];
string txt="";

LOGI("--开始读取文件--");
for(;NULL!=fgets(p,len,file);){
txt+=p;
LOGI("--当前读取到的内容-->%s",txt.data());
}

LOGI("--读取字符串结束，内容:-->%s",txt.data());

if(0!=fclose(file)){
LOGI("--文件关闭失败--");
}else LOGI("--文件关闭成功--");
delete file;
return env->NewStringUTF(txt.data());
}
return env->NewStringUTF("磁磁帅->文件数据读取失败");
}
