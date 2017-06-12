package com.xcc.app4;

/**
 * Created by Administrator on 2017/4/24.
 */
//javah -d jni -classpath E:\AndroidStudioWork\NDKStudy\app4\build\intermediates\classes\debug com.xcc.app4.MyJniThread2
public class MyJniThread2 {
    static {
        System.loadLibrary("myThread");
    }
    public native static void startThread();
    public native static void startThread2();
}
