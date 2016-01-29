package com.qsa.talk;

import android.annotation.SuppressLint;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.view.View.OnClickListener;
import android.os.Bundle;

import android.util.Log;

import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.KeyEvent;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.qsa.player.lib.SerialPortService;
import com.qsa.player.lib.SerialPortServiceForQ;

import com.qsa.comm.libscomm;

@SuppressLint("NewApi")
public class Call extends Activity implements OnClickListener {

	private final String TAG = "qsa_lib";

	private Button btn_call;
	private Button btn_hangup;
	private Button btn_start_video;
	private Button btn_start_audio;

	private EditText edit_number;
	private EditText edit_ip;
	private TextView tv_label;

	private libstalk mlibstalk;
	private libscomm mlibscomm;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_call);

		init_widget_param();
		init_a20_fm2018_param();

		mlibstalk = new libstalk(this);
		SerialPortService.getInstance().open(this);
		//SerialPortServiceForQ.getInstance().open(this);

		mlibscomm = new libscomm();
		mlibscomm.open("/dev/ttyS7");

		/* 显示View后才能监听按键动作 */
		btn_call.setOnClickListener(this);
		btn_hangup.setOnClickListener(this);
		btn_start_video.setOnClickListener(this);
		btn_start_audio.setOnClickListener(this);
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		//close();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		int keyValue;
		keyValue = keyCode - 7;
		if (keyValue == 51) {
			return false;
		}
		Log.v(TAG, "keycode = " + keyValue);
		switch (keyValue) {
		case R.integer.KEY_0:
		case R.integer.KEY_1:
		case R.integer.KEY_2:
		case R.integer.KEY_4:
		case R.integer.KEY_5:
		case R.integer.KEY_7:
		case R.integer.KEY_8:
		case R.integer.KEY_9:
			break;
		case R.integer.KEY_3:
			/* value and up */
			break;
		case R.integer.KEY_6:
			/* value and down */
			break;
		case R.integer.KEY_BS:
			/* delete number */
			break;
		case R.integer.KEY_OK:
			/* ok */
			btn_call.callOnClick();
			break;
		default:
			break;
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public void onClick(View view) {
		switch (view.getId()) {
		case R.id.btn_call:
			Log.i(TAG, "call");
			mlibstalk.start_talk("192.168.10.189", "0606");
			break;
		case R.id.btn_hangup:
			Log.i(TAG, "hangup");
			mlibstalk.stop_talk();
			break;
		case R.id.btn_start_audio:
			Log.i(TAG, "start_audio");
			mlibstalk.start_audio();
			break;
		case R.id.btn_start_video:
			Log.i(TAG, "start_video");
			mlibstalk.start_video();
			break;
		default:
			break;
		}
	}
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		IntentFilter mFilter = new IntentFilter();
		mFilter.addAction("callState");
		this.registerReceiver(mReceiver, mFilter);
	}
	
	private BroadcastReceiver mReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (action.equals("callState")) {
				int state = intent.getIntExtra("callStateID", 11);
				Log.v(TAG, "callState" + state);
				switch (state) {
				case 3:
					tv_label.setText("呼叫中...");
					break;
				case 4:
					tv_label.setText("呼叫失败...");
					break;
				case 1:
					tv_label.setText("占线中...");
					break;
				case 2:
					tv_label.setText("呼叫中...");
					break;
				case 5:
					tv_label.setText("通话中...");
					//InitSurfaceView();
					//InitMediaSharePreference();
					//startRecord();
					break;
				case 6:
					tv_label.setText("呼叫失败...");
					break;
				case 7:
					Toast.makeText(Call.this, "开门中", Toast.LENGTH_SHORT)
							.show();
					break;
				case 8:
					Toast.makeText(Call.this, "开门失败", Toast.LENGTH_SHORT)
							.show();
					break;
				case 9:
					tv_label.setText("挂断成功...");
					//stopRecord();
					break;
				case 10:
					tv_label.setText("挂断失败...");
					//stopRecord();
					break;
				default:
					break;
				}
			}
		}
	};


	private void init_widget_param() {
		btn_call = (Button) this.findViewById(R.id.btn_call);
		btn_hangup = (Button) this.findViewById(R.id.btn_hangup);
		btn_start_video = (Button) this.findViewById(R.id.btn_start_video);
		btn_start_audio = (Button) this.findViewById(R.id.btn_start_audio);

		edit_number = (EditText) this.findViewById(R.id.edt_input_number1);
		edit_ip = (EditText) this.findViewById(R.id.edt_input_ip);
		tv_label = (TextView) this.findViewById(R.id.txt_state);
	}

	private void init_a20_fm2018_param() {

		doCommond("mount -o remount rw /system");
		doCommond("chmod 777 /system/fm2018-1204-6-2.ko");
		doCommond("insmod /system/fm2018-1204-6-2.ko");

		doCommond("mount -o remount rw /system/lib");
		doCommond("chmod 777 /system/lib/fm2018-1204-6-2.ko");
		doCommond("insmod /system/lib/fm2018-1204-6-2.ko");
	}

	public Process su;

	private void doCommond(String str) {
		try {
			su = Runtime.getRuntime().exec("/system/xbin/su");
			Log.i(TAG, str);
			su.getOutputStream().write((str + " \n exit \n").getBytes());
		} catch (Exception e) {
			Log.e(TAG, "doCommond Fail");
		}
	}
}
