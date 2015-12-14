APP_PROJECT_PATH := $(call my-dir)

APP_BUILD_SCRIPT := $(call my-dir)/Android.mk
APP_PLATFORM := android-17
APP_ABI := armeabi-v7a
#APP_ABI := armeabi

TALK_ROOT=$(APP_PROJECT_PATH)/talk
COMM_ROOT=$(APP_PROJECT_PATH)/comm

#YKDROID_ROOT=$(YK-IDBT_ROOT)/ykdroid
#ANDROID_INC=$(YK-IDBT_ROOT)/LP/INC/android
#FRW_ROOT=$(YK-IDBT_ROOT)/LP/INC/android/frameworks/base
#FRW_CORE_ROOT=$(YK-IDBT_ROOT)/LP/INC/android/system/core

NDK_BUILD_PATH := /home/l/android/sdk/ndk-bundle
NDK_LIB_PATH := $(NDK_BUILD_PATH)/platforms/$(APP_PLATEORM)/arch-arm/usr/lib

#YK_IDBT_CFLAGS := -D_YK_S5PV210_ -D_MFC210_ -D_BRUSHIDCARD_SUPPORT -DCODEC_AUDIO_GSM -DDTMF_INBAND_FLOAT -D_CODEC_H264_ -DTARGET_BOARD -DHAVE_CONFIG_H -DVIDEO_ENABLED -DIN_LINPHONE -DOSIP_MT -DNO_FFMPEG -DLOG_MODULE #-D_CAPTURE_PIC_ 

#YK_IDBT_CFLAGS += -DTM_CONFIG_DOWNLOAD

#APP_CFLAGS:=-DDISABLE_NEON 
APP_CFLAGS := -Wno-error=format-security


TALK_C_INCLUDES := \
	$(APP_PROJECT_PATH)/include \
	$(TALK_ROOT)/include

COMM_C_INCLUDES := \
	$(APP_PROJECT_PATH)/include \
	$(COMM_ROOT)/include
	
	#$(YK-IDBT_ROOT)/LP/INC/android/frameworks/base/include/media/stagefright/openmax \
	$(YK-IDBT_ROOT)/NewQS			\
	$(YK-IDBT_ROOT)/NewQS/INC		\
	$(YK-IDBT_ROOT)/NewQS/Audio 	\
	$(YK-IDBT_ROOT)/NewQS/Video 	\
	$(YK-IDBT_ROOT)/NewQS/Capture     \
	$(YK-IDBT_ROOT)/NewQS/Capture/include_cedarv     \
	$(YK-IDBT_ROOT)/NewQS/Capture/include_camera    \
	$(YK-IDBT_ROOT)/NewQS/Capture/include_vencoder  \
	$(YK-IDBT_ROOT)/YKCRT/INC
	
