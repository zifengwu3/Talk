package com.qsa.comm;

import android.util.Log;

/**
 * Created by l on 12/14/15.
 */
public class libscomm {
    private final String TAG = "jni_comm";
    static {
        System.loadLibrary("Comm");
    }

    //native method
    public native int open(String port);
    public native void close();
    public native int send(String data);
    public native int send_thread();
    public native int recvice_thread();

}
