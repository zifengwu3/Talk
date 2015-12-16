//
// Created by l on 11/27/15.
// Modify by l on 12/14/15
//

#include "log_jni.h"
#include "string.h"
#include <jni.h>

/*
 * 设置string处理的编码
 * */
#define ENCODE_GB2312 "GB2312"
#define ENCODE_UTF_16 "UTF-16"
#define ENCODE_UTF_8 "UTF-8"

/*
 * java中的jstring, 转化为c的一个字符数组
 * */
char * Jstring2CStr(JNIEnv * env, jstring jstr)
{
    char * rtn = NULL;

    jclass cls_string = (*env)->FindClass(env, "java/lang/String");
    jstring str_encode = (*env)->NewStringUTF(env, ENCODE_UTF_8);
    jmethodID mid = (*env)->GetMethodID(env, cls_string, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid, str_encode);
    jsize len = (*env)->GetArrayLength(env, barr);
    jbyte * ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    if (len > 0) {
        rtn = (char *)malloc(len + 1);
        memcpy(rtn, ba, len);
        rtn[len] = '\0';
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);  //释放内存

    return rtn;
}


