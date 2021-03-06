LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
CUR_COMM_FOLDER1 := qsaTalk2
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE := qsa_talk
LOCAL_ARM_MODE  := arm
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := $(CUR_COMM_FOLDER1)/include

#Comm Code
LOCAL_SRC_FILES := $(CUR_COMM_FOLDER1)/init.c \
				   $(CUR_COMM_FOLDER1)/cb_function.c \
				   $(CUR_COMM_FOLDER1)/talk.c \
				   $(CUR_COMM_FOLDER1)/timer.c \
				   $(CUR_COMM_FOLDER1)/udp.c
				   
LOCAL_LDFLAGS :=  -ldl -lm -lc  -lz -llog

include $(PREBUILT_STATIC_LIBRARY) 	

include $(CLEAR_VARS)
LOCAL_MODULE := serial_port
LOCAL_SRC_FILES := share_libs/libserial_port.so
include $(PREBUILT_SHARED_LIBRARY) 	

include $(CLEAR_VARS)
LOCAL_MODULE    := Comm
LOCAL_ARM_MODE  := arm
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_C_INCLUDES := $(COMM_C_INCLUDES)

#Tag Flag
MY_LOG_TAG := \"jni_log\"

#Tag Level
ifeq ($APP_OPTIM), release)
	MY_LOG_TAG := MY_LOG_LEVEL_ERROR
else
	MY_LOG_TAG := MY_LOG_LEVEL_VERBOSE
endif

CUR_COMM_FOLDER := comm
#Jni Code
LOCAL_SRC_FILES := jni_load.c \
				   jni_load_comm.c

#Comm Code
LOCAL_SRC_FILES += $(CUR_COMM_FOLDER)/comm.c \
				   $(CUR_COMM_FOLDER)/jni_comm_code.c
				   
LOCAL_LDFLAGS := -L$(NDK_LIB_PATH)
LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := Talk
CUR_TALK_FOLDER := talk
LOCAL_ARM_MODE  := arm
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_C_INCLUDES := $(TALK_C_INCLUDES)

#Jni Code
LOCAL_SRC_FILES := jni_load.c \
				   jni_load_talk.c

#Talk Code
LOCAL_SRC_FILES += $(CUR_TALK_FOLDER)/arp.c \
				   $(CUR_TALK_FOLDER)/init.c \
				   $(CUR_TALK_FOLDER)/sndtools.c \
				   $(CUR_TALK_FOLDER)/talk.c \
				   $(CUR_TALK_FOLDER)/timer.c \
				   $(CUR_TALK_FOLDER)/udp.c \
				   $(CUR_TALK_FOLDER)/video_send.c \
				   $(CUR_TALK_FOLDER)/jni_talk_code.c
				   
LOCAL_LDFLAGS := -L$(NDK_LIB_PATH)
LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)
