//
// Created by fengrui.fr on 17/2/11.
//

#include "com_fr_flac_FrFlac.h"
#include "include/FLAC/ordinals.h"
#include "include/FLAC/stream_encoder.h"
#include "include/FLAC/stream_decoder.h"


FLAC__StreamEncoder *encoder = NULL;
JNIEnv * decoderEnv = NULL;


static void progress_callback(const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data);


static FLAC__StreamDecoderReadStatus read_callback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data);
static FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);
static void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);

/*
 * Class:     com_fr_flac_FrFlac
 * Method:    initEncoder
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_initEncoder
        (JNIEnv * jniEnv, jobject jThis, jstring file) {

    FLAC__bool ok = true;
    FILE * fout;
    FLAC__StreamEncoderInitStatus init_status;
    FLAC__StreamMetadata *metadata[2];
    FLAC__StreamMetadata_VorbisComment_Entry entry;
    unsigned sample_rate = 0;
    unsigned channels = 0;
    unsigned bps = 0;

    char * jniFile = (char*)(*jniEnv)->GetStringUTFChars(jniEnv, file, NULL);

    if((fout = fopen(jniFile, "rb")) == NULL) {
        LOGD("ERROR: opening %s for output\n", jniFile);
        return;
    }
    fclose(fout);

    if((encoder = FLAC__stream_encoder_new()) == NULL) {
        LOGD("ERROR: allocating encoder\n");
        return;
    }

    sample_rate = 44100;
    channels = 1;
    bps = 16;

    ok &= FLAC__stream_encoder_set_verify(encoder, true);
    ok &= FLAC__stream_encoder_set_compression_level(encoder, 5);
    ok &= FLAC__stream_encoder_set_channels(encoder, channels);
    ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, bps);
    ok &= FLAC__stream_encoder_set_sample_rate(encoder, sample_rate);



    if(ok) {
        init_status = FLAC__stream_encoder_init_file(encoder, jniFile, progress_callback, /*client_data=*/NULL);
        if(init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
            LOGD("ERROR: initializing encoder: %s\n", FLAC__StreamEncoderInitStatusString[init_status]);
            ok = false;
        }
    }

    if(!ok) {
        encoder = NULL;
    }
}

/*
 * Class:     com_fr_flac_FrFlac
 * Method:    encode
 * Signature: ([SI)V
 */
JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_encode
        (JNIEnv * jniEnv, jobject jThis, jshortArray audioData, jint audioSize) {

    if(NULL == encoder) {
        LOGD("ERROR: encoder is NULL");
        return;
    }

    FLAC__int16 * buffer = (FLAC__int16*)(*jniEnv)->GetShortArrayElements(jniEnv, audioData, NULL);
    FLAC__int32 pcm[audioSize/2];

    for(int i = 0; i < audioSize/2; i++) {
        pcm[i] = (FLAC__int32)(buffer[2*i+1] << 8 | buffer[2*i]);
//        LOGD("DEBUG: pcm[%d] is %d", i, pcm[i]);
    }
    FLAC__bool ok = FLAC__stream_encoder_process_interleaved(encoder, pcm, audioSize/2);
//    LOGD("DEBUG: FLAC__stream_encoder_process_interleaved result is: %d", ok);

}

/*
 * Class:     com_fr_flac_FrFlac
 * Method:    finishEncode
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_finishEncode
        (JNIEnv * jniEnv, jobject jThis) {

    if(NULL == encoder) {
        return;
    }
    FLAC__stream_encoder_finish(encoder);
    FLAC__stream_encoder_delete(encoder);
}

JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_initDecoder
  (JNIEnv * jniEnv, jobject jThis, jstring file) {

    FLAC__bool ok = true;
    FLAC__StreamDecoder *decoder = 0;
   	FLAC__StreamDecoderInitStatus init_status;
   	FILE * fin;

    char * jniFile = (char*)(*jniEnv)->GetStringUTFChars(jniEnv, file, NULL);

    if((fin = fopen(jniFile, "rb")) == NULL) {
    	fprintf(stderr, "ERROR: opening %s for output\n", jniFile);
    	return;
   	}

   	if((decoder = FLAC__stream_decoder_new()) == NULL) {
   	    return ;
    }
    init_status =  FLAC__stream_decoder_init_stream(
                  	decoder,
                  	read_callback,
                  	NULL,
                  	NULL,
                  	NULL,
                  	NULL,
                  	write_callback,
                  	NULL,
                  	error_callback,
                  	fin
                  );
    if(init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
    	LOGD("ERROR: initializing decoder: %s\n", FLAC__StreamDecoderInitStatusString[init_status]);
    	ok = false;
    }
}

void progress_callback(const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data)
{

}

FLAC__StreamDecoderReadStatus read_callback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data) {

    FILE *file = (FILE*)client_data;
    if(*bytes > 0) {
      *bytes = fread(buffer, sizeof(FLAC__byte), *bytes, file);
      if(ferror(file))
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
      else if(*bytes == 0)
        return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
      else
        return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
    } else {
      return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    }

}

FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data) {

    FLAC__int16 newBuffer[frame->header.blocksize * 2];

    for(int i = 0; i < frame->header.blocksize; i++) {
		newBuffer[2*i] = (FLAC__int16)buffer[0][i];  /* left channel */
		newBuffer[2*i+1] = (FLAC__int16)buffer[1][i];     /* right channel */
	}

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;

}


void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data) {

}
