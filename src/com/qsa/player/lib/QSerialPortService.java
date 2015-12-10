package com.qsa.player.lib;

import android.content.Context;

public interface QSerialPortService {
	
	void open(Context ac);
	
	void close();
	
	void write(byte[] value);
}
