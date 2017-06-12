#include "com_xcc_app5_TCPUtils.h"
#include <android/log.h>
#include <string>
#include <arpa/inet.h>//设置ip地址
#include <unistd.h>//关闭socket
//#include <sys/socket.h>//貌似可以不用

#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/un.h>
#include<stdlib.h>
#include <stddef.h>

#define SERVPORT 1935 //端口号
#define MAXDATASIZE 10    /*每次最大数据传输量 */

#define  LOG_TAG    "--xcc-native-dev--"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

using namespace std;//定义命名空间，string使用

void connectServe() {
    LOGI("--connectServe--");
    int sock_fd;//记录socket
    //1.创建socket
    /**
     * PF_INET ipv4协议
     * SOCK_STREAM 表示使用TCP协议
     * SOCK_DGRAM 表示使用UDP协议
    */
    //if (-1 == (sock_fd = socket(PF_INET, SOCK_STREAM, 0))) {
    //创建本地socket时,需要使用AF_UNIX,否则出现绑定失败
    if (-1 == (sock_fd = socket(AF_UNIX, SOCK_STREAM, 0))) {
        LOGI("--socket创建出错--");
    } else
        LOGI("--socket创建成功--");


    //2.连接ip地址
    /**
     * 通过域名获取地址可使用gethostbyname
     * inet_addr将ip地址字符串转成网络字节序IP
     */
//    struct sockaddr_in serv_addr;
//    serv_addr.sin_family = PF_INET;//设置协议
//    serv_addr.sin_port = htons(SERVPORT);//设置端口
//    serv_addr.sin_addr.s_addr = inet_addr("192.168.1.164");//设置ip地址,需要头文件arpa/inet.h
//    bzero(&(serv_addr.sin_zero), 8);

    //若创建本地socket，使用一下代码
    const char *name = "@xccsocket";//此处抽象命名,用@站位
    /**
   * 本地socket有两种命名方式
   * 一、文件命名：socket会根据此命名创建一个同名的socket文件，客户端连接的时候通过读取该socket文件连接到socket服务端。
   * 这种方式的弊端是服务端必须对socket文件的路径具备写权限，客户端必须知道socket文件路径，且必须对该路径有读权限。
   * 二、抽象命名，这种方式不需要创建socket文件，只需要命名一个全局名字，即可让客户端根据此名字进行连接。
   *注：抽象命名在对地址结构成员sun_path数组赋值的时候，必须把第一个字节置0，即sun_path[0] = 0
   */
    struct sockaddr_un serv_addr;//注意，此处是sockaddr_un，不是sockaddr_in
    serv_addr.sun_family = AF_UNIX;//设置协议
    strcpy(serv_addr.sun_path, name);
    serv_addr.sun_path[0]=0;//抽象命名需要
    //const size_t nameLen = strlen(name);//文件命名只这样使用
    const size_t nameLen = strlen(name) + offsetof(struct sockaddr_un, sun_path);

    if (-1 == connect(sock_fd, (struct sockaddr *) &serv_addr, nameLen)) {//这是连接本地socket使用
//    if (-1 == connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr))) {//这是连接网络socket使用
        LOGI("--socket连接出错--");
    } else
        LOGI("--socket连接成功--");
    //3.发送数据
    /**
     *  sockfd：指定发送端套接字描述符。
     *  buff：存放要发送数据的缓冲区
     *  nbytes:实际要改善的数据的字节数(实际发送的长度)
     *  flags：一般设置为0
     */
    string text = "磁磁帅\n";
    const char *buff = text.data();
    int len = text.length();
    if (send(sock_fd, buff, len, 0) == -1) {
        LOGI("--发送数据失败--");
    } else
        LOGI("--发送数据成功--");
    //4.接收数据
    char buf[MAXDATASIZE + 1];
    int recvbytes = 0;
    /**
*  sockfd：套接字描述符。
*  buf：数据的缓冲区
*  nbytes:缓冲大小
*  flags：一般设置为0
*/
//    这是我原先读取数据的操作
//    if ((recvbytes = recv(sock_fd, buf, MAXDATASIZE, 0)) == -1) {
//        LOGI("--读取数据失败--");
//    } else {
//        LOGI("--读取数据成功--");
//        buf[recvbytes] = '\0';
//        LOGI("--读取到的数据:%s",buf);
//    }
    //当数据较多，缓冲区就装不下，读取方式就要改成如下
    string msg = "";
    for (recvbytes = recv(sock_fd, buf, MAXDATASIZE, 0); ; recvbytes = recv(sock_fd, buf,
                                                                            MAXDATASIZE, 0)) {
        if (recvbytes == -1) {
            LOGI("--读取数据失败--");
            break;
        } else {
            buf[recvbytes] = '\0';
            msg += buf;
            if (recvbytes < MAXDATASIZE) {
                break;
            }
        }
    }
    LOGI("--最终读取到的数据:%s", msg.data());

    LOGI("--断开socket连接--");
//5.关闭socket,需要头文件unistd.h
    close(sock_fd);
}

void Java_com_xcc_app5_TCPUtils_connectServe(JNIEnv * env, jclass
jcl){
connectServe();

}

#define BACKLOG 10    //最大同时连接请求数
int isServe = 0;
int serve_sock_fd;//服务socket

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
    //if (-1 == (sock_fd = socket(PF_INET, SOCK_STREAM, 0))) {
    //在绑定本地socket时,需要使用AF_UNIX,否则出现绑定失败
    if (-1 == (sock_fd = socket(AF_UNIX, SOCK_STREAM, 0))) {
        LOGI("--socket创建出错--");
    } else
        LOGI("--socket创建成功--");
    //2.绑定socket
//    struct sockaddr_in my_addr;
//    my_addr.sin_family = PF_INET;//设置协议
//    my_addr.sin_port = htons(SERVPORT);//设置端口
//    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定到所有地址 inet_addr("127.0.0.1");//设置127只能本地访问

    //若创建本地socket，使用一下代码
    const char *name = "@xccsocket";//此处抽象命名,用@站位
    /**
   * 本地socket有两种命名方式
   * 一、文件命名：socket会根据此命名创建一个同名的socket文件，客户端连接的时候通过读取该socket文件连接到socket服务端。
   * 这种方式的弊端是服务端必须对socket文件的路径具备写权限，客户端必须知道socket文件路径，且必须对该路径有读权限。
   * 二、抽象命名，这种方式不需要创建socket文件，只需要命名一个全局名字，即可让客户端根据此名字进行连接。
   *注：抽象命名在对地址结构成员sun_path数组赋值的时候，必须把第一个字节置0，即sun_path[0] = 0
   */
    unlink(name);//删除原先的socket对象
    struct sockaddr_un my_addr;
    my_addr.sun_family = AF_UNIX;//设置协议
    strcpy(my_addr.sun_path, name);
    my_addr.sun_path[0]=0;//抽象命名需要
    //const size_t nameLen = strlen(name);//文件命名只这样使用
    const size_t nameLen = strlen(name)  + offsetof(struct sockaddr_un, sun_path);

    if (bind(sock_fd, (struct sockaddr *) &my_addr, nameLen) == -1){//这是绑定本地socket使用
//    if (bind(sock_fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {//绑定网络socket使用
        LOGI("--绑定socket失败--");
    } else
        LOGI("--绑定socket成功--");
    //3.开始监听
    if (listen(sock_fd, BACKLOG) == -1) {
        LOGI("--socket监听失败--");
        isServe = 0;
    } else {
        LOGI("--socket监听成功--");
        serve_sock_fd = sock_fd;
    }
    while (isServe) {
        LOGI("--socket开始等待连接--");
        int client_fd;
        //socklen_t sin_size = sizeof(struct sockaddr_in);
        //注意，此处是sockaddr_un，不是sockaddr_in
        socklen_t sin_size = sizeof(struct sockaddr_un);
        if ((client_fd = accept(sock_fd, (struct sockaddr *) &my_addr, &sin_size)) == -1) {
            //尽量不要使用NULL
            //if ((client_fd = accept(sock_fd, (struct sockaddr *) &my_addr, NULL)) == -1) {
            LOGI("--client连接失败--");
            continue;
        } else {
            /**
             * 下面这段应当交由子线程运行
             */
            LOGI("--client连接成功--");
            LOGI("--开始接收数据--");
            string msg = "";
            int readLen = 10;
            char buf[readLen + 1];
            int recvbytes = 0;
            for (recvbytes = recv(client_fd, buf, readLen, 0); ; recvbytes = recv(sock_fd, buf,
                                                                                  readLen, 0)) {
                if (recvbytes == -1) {
                    LOGI("--读取数据失败--");
                    break;
                } else {
                    buf[recvbytes] = '\0';
                    msg += buf;
                    if ('\r' == buf[recvbytes - 1] || '\n' == buf[recvbytes - 1]) break;
                    LOGI("--读取到数据:%s", msg.data());
//                    if (recvbytes < readLen)break;
                }
            }
            LOGI("--最终读取到的数据:%s", msg.data());
            if ("xcc\n" == msg) {
                msg = "--xcc--\n";
            } else msg = "--send-->" + msg;

            const char *buff = msg.data();
            int len = msg.length();
            if (send(client_fd, buff, len, 0) == -1) {
                LOGI("--发送数据失败--");
            } else
                LOGI("--发送数据成功--");

            LOGI("--关闭client_socket连接--");
            close(client_fd);
        }
    }
    LOGI("--关闭socket连接--");
//5.关闭socket,需要头文件unistd.h
    /**
     * 注，此处的关闭，只是关闭服务socket，也就是关闭绑定监听等。
     * 但是client_socket是不会因此关闭
     */
    close(sock_fd);
    serve_sock_fd = 0;
}

void Java_com_xcc_app5_TCPUtils_startServe(JNIEnv * env, jclass
jcl){
startServe();

}
void Java_com_xcc_app5_TCPUtils_stopServe(JNIEnv * env, jclass
jcl){
LOGI("--stopServe--");
isServe = 0;
/**
 * 注：这样操作的本意在close(socket);时，能中断accept()操作，
 * 但是accept()是堵塞式操作，没法中断。
 * 未能百度到解决方案，但别人建议使用select来监听
 */
if(serve_sock_fd)close(serve_sock_fd);
serve_sock_fd = 0;
}