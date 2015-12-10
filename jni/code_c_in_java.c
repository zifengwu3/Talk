#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "log_jni.h"

#define _Included_Jni_Load
#include "jni_load.h"

#include "code_c_in_java.h"

/*
 * Class:     com_qsa_talk_libstalk
 * Method:    start_talk
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V;
 */

JNIEXPORT void JNICALL native_start_talk(JNIEnv *env, jobject obj, jstring ip, jstring number) {
	LOGI(TAG, ( *env )->NewStringUTF(env, "Good Hello World!"));
	return;
}

/*
 * Class:     com_qsa_talk_libstalk
 * Method:    start_talk
 * Signature: ()V;
 */
JNIEXPORT void JNICALL native_stop_talk(JNIEnv *env, jobject obj) {
	LOGI(TAG, ( *env )->NewStringUTF(env, "GoodBye World!"));
	return;
}
