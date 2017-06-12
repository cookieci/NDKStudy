package com.xcc.app3;

/**
 * Created by Administrator on 2017/4/19.
 */
//javah -d jni -classpath E:\AndroidStudioWork\NDKStudy\app3\build\intermediates\classes\debug com.xcc.app3.MyJni
public class MyJni {
    static {
        System.loadLibrary("myJni");
    }

    public native static void fileW();

    public native static String fileR();
}
