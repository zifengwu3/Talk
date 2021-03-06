#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "log_jni.h"

#define _Included_Jni_Load
#include "jni_load.h"

#include "jni_comm_code.h"

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: (Ljava/lang/String;)I;
 */

JNIEXPORT jint JNICALL native_open(JNIEnv *env, jobject obj, jstring port) {
	char * serialport;
	serialport = Jstring2CStr(env, port);
	LOGI("open comm : %s", serialport);
	OpenComm(serialport, 9600, 8, 1, 'N');
	return 1;
}

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: ()V;
 */
JNIEXPORT void JNICALL native_close(JNIEnv *env, jobject obj) {
	LOGI("close comm");
	CloseComm();
	return;
}

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: (Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL native_send(JNIEnv *env, jobject obj, jstring data) {
	LOGI("send data");
	return 1;
}

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: ()I
 */
JNIEXPORT jint JNICALL native_send_thread(JNIEnv *env, jobject obj) {
	LOGI(( *env )->NewStringUTF(env, "send thread"));
	return 1;
}

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: ()I
 */
JNIEXPORT jint JNICALL native_recvice_thread(JNIEnv *env, jobject obj) {
	LOGI(( *env )->NewStringUTF(env, "recvice thread"));
	return 1;
}

