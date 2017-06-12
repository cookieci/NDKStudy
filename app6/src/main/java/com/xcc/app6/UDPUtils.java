package com.xcc.app6;

/**
 * Created by Administrator on 2017/5/10.
 */
//javah -d jni -classpath E:\AndroidStudioWork\NDKStudy\app6\build\intermediates\classes\debug com.xcc.app6.UDPUtils
public class UDPUtils {
    static {
        System.loadLibrary("UDPUtils");
    }

    public native static void connectServe();

    public native static void startServe();

    public native static void stopServe();
}
