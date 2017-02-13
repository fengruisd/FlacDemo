LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	bitmath.c \
	bitreader.c \
	bitwise.c \
	bitwriter.c \
	cpu.c \
	crc.c \
	fixed.c \
	fixed_intrin_sse2.c \
	fixed_intrin_ssse3.c \
	float.c \
	format.c \
	framing.c \
	lpc.c \
	lpc_intrin_avx2.c \
	lpc_intrin_sse.c \
	lpc_intrin_sse2.c \
	lpc_intrin_sse41.c \
	memory.c \
	md5.c \
	metadata_iterators.c \
	metadata_object.c \
	ogg_decoder_aspect.c \
	ogg_encoder_aspect.c \
	ogg_helper.c \
	ogg_mapping.c \
	stream_decoder.c \
	stream_encoder.c \
	stream_encoder_framing.c \
	stream_encoder_intrin_avx2.c \
	stream_encoder_intrin_sse2.c \
	stream_encoder_intrin_ssse3.c \
	window.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/.. \
	$(LOCAL_PATH)/../include \
	$(LOCAL_PATH)/../ogg

LOCAL_CFLAGS += -DHAVE_CONFIG_H -DFLAC__NO_MD5 -DFLAC__INTEGER_ONLY_LIBRARY
LOCAL_CFLAGS += -D_REENTRANT -DPIC -DU_COMMON_IMPLEMENTATION -fPIC
LOCAL_CFLAGS += -O3 -funroll-loops -finline-functions

#LOCAL_ARM_MODE := arm

LOCAL_MODULE := libflac

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)