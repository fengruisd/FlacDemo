LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LIBFLAC_DIR := libflac

LOCAL_SRC_FILES:= \
	$(LIBFLAC_DIR)/bitmath.c \
	$(LIBFLAC_DIR)/bitreader.c \
	$(LIBFLAC_DIR)/bitwise.c \
	$(LIBFLAC_DIR)/bitwriter.c \
	$(LIBFLAC_DIR)/cpu.c \
	$(LIBFLAC_DIR)/crc.c \
	$(LIBFLAC_DIR)/fixed.c \
	$(LIBFLAC_DIR)/fixed_intrin_sse2.c \
	$(LIBFLAC_DIR)/fixed_intrin_ssse3.c \
	$(LIBFLAC_DIR)/float.c \
	$(LIBFLAC_DIR)/format.c \
	$(LIBFLAC_DIR)/framing.c \
	$(LIBFLAC_DIR)/lpc.c \
	$(LIBFLAC_DIR)/lpc_intrin_avx2.c \
	$(LIBFLAC_DIR)/lpc_intrin_sse.c \
	$(LIBFLAC_DIR)/lpc_intrin_sse2.c \
	$(LIBFLAC_DIR)/lpc_intrin_sse41.c \
	$(LIBFLAC_DIR)/memory.c \
	$(LIBFLAC_DIR)/md5.c \
	$(LIBFLAC_DIR)/metadata_iterators.c \
	$(LIBFLAC_DIR)/metadata_object.c \
	$(LIBFLAC_DIR)/ogg_decoder_aspect.c \
	$(LIBFLAC_DIR)/ogg_encoder_aspect.c \
	$(LIBFLAC_DIR)/ogg_helper.c \
	$(LIBFLAC_DIR)/ogg_mapping.c \
	$(LIBFLAC_DIR)/stream_decoder.c \
	$(LIBFLAC_DIR)/stream_encoder.c \
	$(LIBFLAC_DIR)/stream_encoder_framing.c \
	$(LIBFLAC_DIR)/stream_encoder_intrin_avx2.c \
	$(LIBFLAC_DIR)/stream_encoder_intrin_sse2.c \
	$(LIBFLAC_DIR)/stream_encoder_intrin_ssse3.c \
	$(LIBFLAC_DIR)/window.c \
	frflac.c

LOCAL_C_INCLUDES += \
	$(LIBFLAC_DIR)/include \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/ogg \
	com_fr_flac_FrFlac.h

LOCAL_CFLAGS += -DHAVE_CONFIG_H -DFLAC__NO_MD5 -DFLAC__INTEGER_ONLY_LIBRARY
LOCAL_CFLAGS += -D_REENTRANT -DPIC -DU_COMMON_IMPLEMENTATION -fPIC
LOCAL_CFLAGS += -O3 -funroll-loops -finline-functions
LOCAL_LDLIBS :=  -L$(SYSROOT)/usr/lib -llog

#LOCAL_ARM_MODE := arm

LOCAL_MODULE := libflac

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

#include $(LOCAL_PATH)/libflac/Android.mk