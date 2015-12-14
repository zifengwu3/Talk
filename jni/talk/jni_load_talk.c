//
// Created by l on 11/27/15.
// Modify by l on 12/14/15
//

#include "log_jni.h"

#define _Included_com_qsatalk_cinjava
#include "jni_talk_code.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

/*
 * 获取数组大小
 * */
#define NELEM(x) ((int)(sizeof(x) / sizeof((x)[0])))

/*
 * 设置string处理的编码
 * */
/*
 * 指定要注册的类，对应完整的java类名
 * */
#define JNIREG_CLASS "com/qsa/talk/libstalk"

JNIEXPORT jint JNI_OnLoad(JavaVM * vm, void * reserved) {
    JNIEnv * env = NULL;
    jint result = (-1);
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }

    register_ndk_load(env);
    return JNI_VERSION_1_6;
}

/*
 * 注册native方法到java中
 * */
static int registerNativeMethods(JNIEnv *env, const char * className,
        JNINativeMethod * gMethods, int numMethods) {
    jclass clazz;
    clazz = (* env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    if ((* env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
        return  JNI_FALSE;
    }
    return  JNI_TRUE;
}

/*
 * Java和JNI函数绑定
 * */
static JNINativeMethod method_table[] = {
        {"start_talk", "(Ljava/lang/String;Ljava/lang/String;)V", (void *)native_start_talk},
        {"stop_talk", "()V", (void *)native_stop_talk},
        {"start_video", "()V", (void *)native_start_video},
        {"start_audio", "()V", (void *)native_start_audio},
};

/*
 * 调用注册方法
 * */
int register_ndk_load(JNIEnv * env) {
    return registerNativeMethods(env, JNIREG_CLASS, method_table, NELEM(method_table));
}



