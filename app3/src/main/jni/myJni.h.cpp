#include "com_xcc_app3_MyJni.h"
#include <string>
#include <android/log.h>
#include <stdio.h>

using namespace std;//定义命名空间，string使用

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void Java_com_xcc_app3_MyJni_fileW(JNIEnv * env, jclass
jc){//写入
FILE *file=fopen("/data/data/com.xcc.app3/jniRW.txt","w+");
/**
 * 注:w只读，w+读写双模式
 * 双模式下，在读写转化时，需要先调用fflush 刷新缓冲区
 */
if(NULL==file){
LOGI("--文件打开失败--");
}else{
LOGI("--文件打开成功--");
//string str="--这是磁磁帅写入的文件数据--";
string str="--this is cicishuai write file data--";
int len=str.length();
char p[len+1];
for(int i=0;i<str.length();i++)
p[i] = str[i];
p[len] = '\0';
if(len!=fwrite(p,sizeof(char),len,file)){
LOGI("--文件写入出错--长度:%d",len);
}else LOGI("--文件写入成功--");
if(0!=fclose(file)){
LOGI("--文件关闭失败--");
}else LOGI("--文件关闭成功--");
delete file;
/**
 * 注：文件读写时，不会立即写入到文件
 * 会先写入到缓冲区，
 * fflush和fclose操作时才会写入到文件
 */
}
}

jstring Java_com_xcc_app3_MyJni_fileR(JNIEnv * env, jclass
jc){//读取
/**
 * 注：读取文件时应当使用"r+",
 * 使用"w+"时，可能出现读取不到数据
 */
FILE *file=fopen("/data/data/com.xcc.app3/jniRW.txt","r+");
if(NULL==file){
LOGI("--文件打开失败--");
}else{
LOGI("--文件打开成功--");
int len=10;
char p[len+1];
string txt="";
LOGI("--开始读取文件--");
/**
 * 注：fread读取数据易发生乱码，个人尚无解决方法，不建议使用
 * 建议使用fgets();
 * fread不是用于字符串读写函数
 */
for(int idx=fread(p,sizeof(char),len,file);idx>0;idx=fread(p,sizeof(char),len,file)){
p[idx+1]='\0';//这条是用于解决乱码的，但是没有任何效果
string sTmp=p;
txt+=sTmp;
LOGI("--当前读取到的内容-->%s",txt.data());//注：此处需要的是 *string 即 char[],所以得传txt.data()
}
LOGI("--读取字符串结束，内容:-->%s",txt.data());

if(0!=fclose(file)){
LOGI("--文件关闭失败--");
}else LOGI("--文件关闭成功--");
delete file;//我认为fclose应该会回收内存，但是为了安全起见，我还是写上delete
/**
 * 注：当txt中出现乱码时，NewStringUTF就会发生异常。
 * 然后程序崩溃
 */
//return env->NewStringUTF(txt.data());
}
return env->NewStringUTF("磁磁帅->文件数据读取失败");
}
