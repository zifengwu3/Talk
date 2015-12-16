package com.qsa.talk;

import android.content.Intent;
import android.content.Context;
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
    
    public void Recv_Talk_Line_Use(int isBusy) {

		if (isBusy == 1) {
			sendCallState(1);
		} else {
			sendCallState(2);
		}

	}

	public void Recv_Talk_Call_Answer(int isCallSuccess) {
		if (isCallSuccess == 1) {
			sendCallState(3);
		} else {
			sendCallState(4);
		}
	}

	public void Recv_Talk_Call_Start(int isConnectSuccess) {
		if (isConnectSuccess == 1) {
			sendCallState(5);
		} else {
			sendCallState(6);
		}
	}

	public void Recv_Talk_Open_Lock(int isOpenSuccess) {
		if (isOpenSuccess == 1) {
			sendCallState(7);
		} else {
			sendCallState(8);
		}
	}

	public void Recv_Talk_Call_End(int ishangupSuccess) {
		if (ishangupSuccess == 1) {
			sendCallState(9);
		} else {
			sendCallState(10);
		}
	}
	
	private Context context;
	
	public libstalk(Context context) {
		this.context = context;
	}
	
	private void sendCallState(int state){
		
		Intent intent = new Intent();
		intent = new Intent("callState");
		intent.putExtra("callStateID", state);
		context.sendBroadcast(intent);
		
	}

}
