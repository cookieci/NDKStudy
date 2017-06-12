#include "com_xcc_app3_MyJni3.h"
#include <string>
#include <android/log.h>
#include <stdio.h>

using namespace std;

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void charFileW() {//字符方式写入文件
    FILE *file = fopen("/data/data/com.xcc.app3/jniRW3.txt", "w+");
    if (NULL == file) {
        LOGI("--文件打开失败--");
    } else {
        LOGI("--文件打开成功--");
        string str = "--这是磁磁帅写入的文件数据--";
        //string str="--this is cicishuai write file data--";
        int len = str.length();
        const char *cs;
        cs = str.data();
        for (int idx = 0; idx < len; idx++) {
            if (EOF == fputc(cs[idx], file)) {
                LOGI("--文件写入出错--");
                break;
            }
            else
                LOGI("--文件写入字符成功--");
        }

        if (0 != fclose(file)) {
            LOGI("--文件关闭失败--");
        } else
            LOGI("--文件关闭成功--");
        delete file;
    }
}
void printFileW(){//格式化写入文件
    FILE *file = fopen("/data/data/com.xcc.app3/jniRW4.txt", "w+");
    if (NULL == file) {
        LOGI("--文件打开失败--");
    } else {
        LOGI("--文件打开成功--");
        //string str="--this is cicishuai write file data--";
        /**
         * 注：需要在%s前后添加空格,否则会导致不能正常格式化读取数据
         */
        if(0>fprintf(file,"--这是 %s 写入的文件数据 %d --","磁磁帅",1111)){
            LOGI("--写入数据出错--");
        }
        if (0 != fclose(file)) {
            LOGI("--文件关闭失败--");
        } else
            LOGI("--文件关闭成功--");
        delete file;
    }
}

void Java_com_xcc_app3_MyJni3_fileW(JNIEnv * env, jclass
jc){//写入
LOGI("--字符方式写入文件-->");
charFileW();
LOGI("--格式化数据写入文件-->");
printFileW();
}

string charFileR() {//字符方式读文件
    FILE *file = fopen("/data/data/com.xcc.app3/jniRW3.txt", "r+");
    string txt = "";
    if (NULL == file) {
        LOGI("--文件打开失败--");
    } else {
        LOGI("--文件打开成功--");
        char tmpC[1];
        for (char retInt = fgetc(file); ; retInt = fgetc(file)) {
            if (EOF == retInt) {
                LOGI("--读文件出错 或 读取文件结束--");
                break;
            }
            tmpC[0] = retInt;
            txt += tmpC;
            LOGI("--当前读取到的内容-->%s", txt.data());
        }
        if (0 != fclose(file)) {
            LOGI("--文件关闭失败--");
        } else
            LOGI("--文件关闭成功--");
        delete file;
        return txt;
    }
    /**
     * 注:此处返回string，
     * 但是 "xxxx" 这是不是string,这是char[];
     * 所以不能直接写 return "xxxx";
     */
    txt = "磁磁帅->文件数据读取失败";
    return txt;
}
string printFileR() {//格式化读文件
    FILE *file = fopen("/data/data/com.xcc.app3/jniRW4.txt", "r+");
    string txt = "";
    if (NULL == file) {
        LOGI("--文件打开失败--");
    } else {
        LOGI("--文件打开成功--");
        string str = "--这是 %s 写入的文件数据 %d --";
        int i;
        char ch[9];
        /**
         * fscanf返回的是读取到的项目数
         * 此处2表示读取2个项目。
         */
        if(2!=fscanf(file,str.data(),ch,&i)){
            LOGI("--读文件出错--");
        }else{
            txt=ch;
            LOGI("--读取到的数据[%s][%d]--",ch,i);
            return txt;
        }
    }
    txt = "磁磁帅->文件数据读取失败";
    return txt;
}

jstring Java_com_xcc_app3_MyJni3_fileR(JNIEnv * env, jclass
jc){//读取
LOGI("--字符方式读文件-->");
string cString=charFileR();
LOGI("--格式化读文件-->");
cString+=printFileR();
return env->NewStringUTF(cString.data());
}
