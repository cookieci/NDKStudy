package com.xcc.ndkstudy;

/**
 * Created by Administrator on 2017/4/18.
 */
//javah -d jni -classpath E:\AndroidStudioWork\NDKStudy\app\build\intermediates\classes\debug com.xcc.ndkstudy.GetString
public class GetString {
    static {
        System.loadLibrary("xccJnis");
    }

    public native String stringToJni();
}
