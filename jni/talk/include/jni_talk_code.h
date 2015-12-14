/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_qsa_comm_libscomm */

#ifndef _Included_com_qsacomm_cinjava
#define _Included_com_qsacomm_cinjava
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: (Ljava/lang/String;)I;
 */

JNIEXPORT jint JNICALL native_open(JNIEnv *env, jobject obj, jstring port);

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: ()V;
 */
JNIEXPORT void JNICALL native_close(JNIEnv *env, jobject obj);

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: (Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL native_send(JNIEnv *env, jobject obj, jstring data);

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: ()I
 */
JNIEXPORT jint JNICALL native_send_thread(JNIEnv *env, jobject obj);

/*
 * Class:     com_qsa_comm_libscomm
 * Method:    start_comm
 * Signature: ()I
 */
JNIEXPORT jint JNICALL native_recvice_thread(JNIEnv *env, jobject obj);


#else

extern JNIEXPORT jint JNICALL native_open(JNIEnv *env, jobject obj, jstring port);
extern JNIEXPORT void JNICALL native_close(JNIEnv *env, jobject obj);
extern JNIEXPORT jint JNICALL native_send(JNIEnv *env, jobject obj, jstring data);
extern JNIEXPORT jint JNICALL native_send_thread(JNIEnv *env, jobject obj);
extern JNIEXPORT jint JNICALL native_recvice_thread(JNIEnv *env, jobject obj);

#endif
#ifdef __cplusplus
}
#endif