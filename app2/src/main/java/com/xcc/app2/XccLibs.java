package com.xcc.app2;

/**
 * Created by Administrator on 2017/4/19.
 */
//javah -d jni -classpath E:\AndroidStudioWork\NDKStudy\app2\build\intermediates\classes\debug com.xcc.app2.XccLibs
public class XccLibs {
    static {
        System.loadLibrary("xccJnis");
    }

    public native void cRun();
}
