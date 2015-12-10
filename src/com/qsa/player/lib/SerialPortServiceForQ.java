package com.qsa.player.lib;

import android.content.Context;
import android.content.SharedPreferences.Editor;
import android.os.Handler;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.InvalidParameterException;
import java.util.Vector;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SerialPortServiceForQ implements QSerialPortService {

	private static String TAG = "serial_qsa";

	private SerialPort mSerialPort;
	private OutputStream mOutputStream;
	private InputStream mInputStream;

	// 发包超时次数
	private int timeOutCount = 0;
	// 巡检异常计数
	public static int inspectionCount = 0;
	// 串口开关
	private boolean serialportSwith = true;
	private static SerialPortServiceForQ mSerialPortService = null;

	public Context ac;
	private ReadThread mReadThread;
	private static byte Cyclic = 0x00;

	private SerialPortServiceForQ() {
	}

	public static SerialPortServiceForQ getInstance() {
		if (mSerialPortService == null) {
			mSerialPortService = new SerialPortServiceForQ();
		}
		return mSerialPortService;
	}

	@Override
	public void open(final Context ac) {
		// TODO Auto-generated method stub
		serialportSwith = true;
		inspectionCount = 0;
		Cyclic = 0X00;
		try {

			this.ac = ac;
			Log.e(TAG, "开启485串口");
			mSerialPort = new SerialPort(new File("/dev/ttyS7"), 9600, 0);// COM0，波特率9600
			mOutputStream = mSerialPort.getOutputStream();
			mInputStream = mSerialPort.getInputStream();

			/* Create a receiving thread */
			mReadThread = new ReadThread();
			mReadThread.setPriority(Thread.MAX_PRIORITY);
			mReadThread.start();

		} catch (SecurityException e) {
			Log.e(TAG, "Error Security Exception");
			// DisplayError(R.string.error_security);
		} catch (IOException e) {
			Log.e(TAG, "Error UnKnown Exception");
			// DisplayError(R.string.error_unknown);
		} catch (InvalidParameterException e) {
			Log.e(TAG, "Error Invalid Parameter Exception");
			// DisplayError(R.string.error_configuration);
		}

	}

	@Override
	public void close() {
		// TODO Auto-generated method stub
		serialportSwith = false;

		if (mReadThread != null) {
			mReadThread.interrupt();
		}

		mSerialPort = null;
	}

	@Override
	public synchronized void write(byte[] value) {
		// TODO Auto-generated method stub
		byte[] data;
		data = value;
		try {
			Log.v(TAG, "开始写入");
			SerialPortSupport.printHexString(data);
			mOutputStream.write(data);
			Log.v(TAG, "结束写入");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.v(TAG, "写入错误");
			e.printStackTrace();
		}
	}

	private boolean iscomplete = true;
	private int packLength;
	private byte[] packageBuf = new byte[1024];
	private int hadGetLength;

	private class ReadThread extends Thread {

		@Override
		public void run() {
			super.run();

			while (serialportSwith) {
				int size;
				try {
					byte[] buffer = new byte[1024];
					if (mInputStream == null) {
						Log.e(TAG, "mInputSteam is fail");
						return;
					}

					size = mInputStream.read(buffer);

					if (size > 0) {
						Log.v(TAG, "size: " + size);
						// onDataReceived(buffer, size);
					}
				} catch (IOException e) {
					e.printStackTrace();
					Log.e(TAG, "read IOException fail");
					return;
				}

			}
		}
	}

	/**
	 * timeID_a timeID_b 用于计算信息获取的时间
	 */
	private long timeID_a;
	private long timeID_b;

	// 接收数据处理
	protected void onDataReceived(byte[] buffer, int size) {
		byte[] receiveData = null;
		String str = "";
		if (!iscomplete) {
			System.arraycopy(buffer, 0, packageBuf, hadGetLength, size);
			hadGetLength += size;

			if (hadGetLength >= packLength) // 收到包的长度大于包内数据长度
			{
				Log.i(TAG, "======包接收完整===");
				iscomplete = true;
				receiveData = new byte[packLength];
				System.arraycopy(packageBuf, 0, receiveData, 0, packLength);
				sendToDoparse(receiveData);

			}
			return;
		}
		for (int i = 0; i < size; i++) {
			if (buffer[i] == (byte) 0xAD) {
				Log.v(TAG, "找到匹配的起始位");
				packLength = SerialPortSupport.getCount(new byte[] { buffer[i + 1], buffer[i + 2] });
				// 判断指定长度是否大于实际有效包的的长度
				int relength = size - i; // 从 0xAD 以后的数据长度
				if (packLength > relength) {
					// 包还未接收完整
					timeID_a = System.currentTimeMillis();
					Log.i(TAG, "======包未接收完整 包长度" + packLength + "＝＝＝＝");

					iscomplete = false;
					System.arraycopy(buffer, i, packageBuf, 0, relength);
					hadGetLength = relength;
					return;
				}
				if (packLength < 1024 && packLength >= 7) {

				} else {
					Log.e(TAG, "读取数据长度有问题");
					return;
				}
				receiveData = new byte[packLength];
				for (int j = 0; j < packLength; j++) {
					receiveData[j] = buffer[j + i];
					str = str + " " + SerialPortSupport.HexString(receiveData[j]);
				}
				sendToDoparse(receiveData);
				break;
			}

		}
	}

	public void sendToDoparse(byte[] receiveData) {
		// 将读取的内容传送到解析线程中
		if (receiveData != null) {
			Log.d(TAG, "size safe");
			// 开始计时，性能测试用nanoTime会更精确，因为它是纳秒级的
			long startTime = System.nanoTime();
			doParse(receiveData, ac);
			// 停止计时
			long endTime = System.nanoTime();
			// 耗时
			long spendTime = (endTime - startTime);
			Log.d(TAG, "deal data use " + spendTime + "ns");

		}
	}

	// SPI解析器
	public void doParse(byte[] value, Context ac) {
		Log.i(TAG, "----doParse----" + value.length);
		// 校验是否通过
		boolean isNext = checkValue(value);

		// 确认协议方向 true为发送 false为回复
		boolean orient = checkOrient(value[5]);

		if (isNext) {

			// 校验成功
			Log.d(TAG, "校验成功");
			inspectionCount = 0;

			// MCU向ARM发送执行事件
			if (orient) {
				Log.d(TAG, "接收ARM发送执行事件");
				// 如果循环码相同则为已执行过事件 抛弃重复数据
				if (Cyclic != value[3]) {

					Cyclic = value[3];
					switch (value[4]) {
					// 按键信息上传
					case 0x04:
						Log.d(TAG, "按键信息上传");
						SerialPortInfoUpload.getInstance().btn_parse(value[6]);
						break;
					}
				} else {
					Log.d(TAG, "该事件已经被执行");
					byte[] now = new byte[] { (byte) 0xAD, 0x00, 0x08, Cyclic, 0x04, 0x02, 0x01, 0x00 };
					now[now.length - 1] = SerialPortSupport.getCheckValue(now);
					write(now);
				}
			} else {
				Log.d(TAG, "向ARM发送回复事件");
				// 如果循环码相同则为已回复过事件 抛弃重复数据
				if (Cyclic != value[3]) {

					Cyclic = value[3];
					// 检测是否为等待列表需要的回复
					switch (value[4]) {
					// 独立门禁向ARM发送协议
					case 0x05:
						Log.d(TAG, "独立门禁向ARM发送协议");
						break;
					default:
						break;
					}
				} else {
					Log.d(TAG, "该回复已经执行");
				}
			}
		} else {
			// 校验失败 抛弃数据
			Log.d(TAG, "校验失败");
		}
	}

	/**
	 * 
	 * @param value
	 *            SPI通讯传入的内容
	 * 
	 * @return 校验结果是否成功
	 * 
	 */
	public static boolean checkValue(byte[] value) {
		// SerialPortSupport.printHexString(value);
		// 校验头
		if (!SerialPortSupport.HexString(value[0]).equals("BD")) {
			return false;
		}

		// 校验校验码
		if (SerialPortSupport.getUnsignedByte(SerialPortSupport.getCheckValue(value)) != SerialPortSupport
				.getUnsignedByte(value[value.length - 1])) {
			Log.d(TAG, SerialPortSupport.getUnsignedByte(SerialPortSupport.getCheckValue(value)) + "crc result");
			Log.d(TAG, SerialPortSupport.getUnsignedByte(value[value.length - 1]) + "crc");
			return false;
		}
		return true;
	}

	/**
	 * 
	 * @param value
	 *            SPI通讯传入的内容
	 * 
	 * @return 确认协议方向 true为发送 false为回复
	 * 
	 */
	public static boolean checkOrient(byte value) {
		if (value == 0x01) {
			return true;
		} else {
			return false;
		}
	}
}
