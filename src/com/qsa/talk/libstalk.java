package com.qsa.talk;

import android.util.Log;

/**
 * Created by l on 11/29/15.
 */
public class libstalk {
    private final String TAG = "jni_talk";
    static {
        System.loadLibrary("Talk");
    }

    //native method
    public native void start_talk(String ip,String number);
    public native void stop_talk();
    public native void start_audio();
    public native void start_video();

}
