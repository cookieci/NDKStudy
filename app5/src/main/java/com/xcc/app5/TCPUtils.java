package com.xcc.app5;

/**
 * Created by Administrator on 2017/4/27.
 */
//javah -d jni -classpath E:\AndroidStudioWork\NDKStudy\app5\build\intermediates\classes\debug com.xcc.app5.TCPUtils
public class TCPUtils {
    static {
        System.loadLibrary("TCPUtils");
    }

    public native static void connectServe();

    public native static void startServe();

    public native static void stopServe();
}
