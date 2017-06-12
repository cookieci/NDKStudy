#include "com_xcc_app6_UDPUtils.h"
#include <android/log.h>
#include <string>
#include <arpa/inet.h>//设置ip地址
#include <unistd.h>//关闭socket

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

using namespace std;//定义命名空间，string使用

#define SERVPORT 1712 //端口号
#define MAXDATASIZE 10    /*每次最大数据传输量 */

void connectServe() {
    LOGI("--connectServe--");
    int sock_fd;//记录socket
    //1.创建socket
    /**
     * PF_INET ipv4协议
     * SOCK_STREAM 表示使用TCP协议
     * SOCK_DGRAM 表示使用UDP协议
    */
    if (-1 == (sock_fd = socket(PF_INET, SOCK_DGRAM, 0))) {
        LOGI("--socket创建出错--");
    } else
        LOGI("--socket创建成功--");

    /* 填写sockaddr_in*/
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));//将内存(字符串)前n个字节清零
    addr.sin_family = PF_INET;
    addr.sin_port = htons(SERVPORT);
    addr.sin_addr.s_addr = inet_addr("192.168.137.1");
    int addr_len = sizeof(struct sockaddr_in);
    //2.发送信息
    string text = "this is rookieCi\n";
    const char *buff = text.data();
    int len = text.length();
    int sendret = sendto(sock_fd, buff, len, 0, (struct sockaddr *) &addr, addr_len);
    if (sendret == -1) {
        LOGI("--发送信息失败--");
    } else {
        LOGI("--发送信息成功--");
    }
    //3.接收数据
    string msg = "";
    char buf[MAXDATASIZE + 1];
    socklen_t slen = addr_len;
    LOGI("--开始读取数据--");
    for (len = recvfrom(sock_fd, buf, MAXDATASIZE, 0, (struct sockaddr *) &addr,
                        &slen); ; len = recvfrom(sock_fd, buf, MAXDATASIZE, 0,
                                                 (struct sockaddr *) &addr, &slen)) {
        if (len == -1) {
            LOGI("--读取数据失败--");
            break;
        } else {
            buf[len + 1] = '\0';
            msg += buf;
            if (len < MAXDATASIZE) {
                break;
            }
        }
    }
    LOGI("--最终读取到的数据:%s", msg.data());
    LOGI("--断开socket连接--");
    //关闭socket,需要头文件unistd.h
    close(sock_fd);
}

void Java_com_xcc_app6_UDPUtils_connectServe(JNIEnv * env, jclass
jcl){
connectServe();

}
//#define BACKLOG 10    //最大同时连接请求数
int isServe = 0;
int serve_sock_fd;

//服务socket
void startServe() {
    LOGI("--startServe--");
    if (isServe) {
        LOGI("--已开启Serve--");
        return;
    }
    isServe = 1;
    int sock_fd;//记录socket
    //1.创建socket
    /**
     * PF_INET ipv4协议
     * SOCK_STREAM 表示使用TCP协议
     * SOCK_DGRAM 表示使用UDP协议
    */
    if (-1 == (sock_fd = socket(PF_INET, SOCK_DGRAM, 0))) {
        LOGI("--socket创建出错--");
    } else
        LOGI("--socket创建成功--");
    //2.绑定socket
    struct sockaddr_in my_addr;
    my_addr.sin_family = PF_INET;//设置协议
    my_addr.sin_port = htons(SERVPORT);//设置端口
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定到所有地址 inet_addr("127.0.0.1");//设置127只能本地访问
    if (bind(sock_fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
        LOGI("--绑定socket失败--");
    } else
        LOGI("--绑定socket成功--");
    while (isServe) {
        socklen_t sin_size = sizeof(struct sockaddr_in);
        LOGI("--开始接收数据--");
        string msg = "";
        char buf[MAXDATASIZE + 1];
        for (int len = recvfrom(sock_fd, buf, MAXDATASIZE, 0, (struct sockaddr *) &my_addr,
                                &sin_size); ;
             len = recvfrom(sock_fd, buf, MAXDATASIZE, 0, (struct sockaddr *) &my_addr,
                            &sin_size)) {
            if (len == -1) {
                LOGI("--读取数据失败--");
                break;
            } else {
                buf[len + 1] = '\0';
                msg += buf;
                if (len < MAXDATASIZE) {
                    break;
                }
            }
        }
        LOGI("--最终读取到的数据:%s", msg.data());
        LOGI("客户端地址:%s\n" , inet_ntoa(my_addr.sin_addr));
//        my_addr.sin_port=45062;
        LOGI("客户端端口:%d\n" , my_addr.sin_port);
        if ("xcc\n" == msg) {
            msg = "--xcc--\n";
        } else msg = "--send-->" + msg;

        const char *buff = msg.data();
        int len = msg.length();
        int addr_len = sizeof(struct sockaddr_in);
//        sendto(sock_fd, buff, len, 0, (struct sockaddr *) &my_addr, addr_len);
        if (sendto(sock_fd, buff, len, 0, (struct sockaddr *) &my_addr, sin_size) == -1) {
            LOGI("--发送数据失败--");
        } else
            LOGI("--发送数据成功--");
    }
    LOGI("--关闭socket连接--");
//关闭socket,需要头文件unistd.h
    close(sock_fd);
    serve_sock_fd = 0;
}

void Java_com_xcc_app6_UDPUtils_startServe(JNIEnv * env, jclass
jcl){
startServe();

}

void Java_com_xcc_app6_UDPUtils_stopServe(JNIEnv * env, jclass
jcl){
LOGI("--stopServe--");
isServe = 0;
if(serve_sock_fd)
close(serve_sock_fd);
serve_sock_fd = 0;
}