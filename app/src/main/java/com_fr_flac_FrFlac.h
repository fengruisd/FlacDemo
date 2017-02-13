/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_fr_flac_FrFlac */

#ifndef _Included_com_fr_flac_FrFlac
#define _Included_com_fr_flac_FrFlac
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_fr_flac_FrFlac
 * Method:    initEncoder
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_initEncoder
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_fr_flac_FrFlac
 * Method:    encode
 * Signature: ([SI)V
 */
JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_encode
  (JNIEnv *, jobject, jshortArray, jint);

/*
 * Class:     com_fr_flac_FrFlac
 * Method:    finishEncode
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_finishEncode
  (JNIEnv *, jobject);

/*
 * Class:     com_fr_flac_FrFlac
 * Method:    initDecoder
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_fr_flac_FrFlac_initDecoder
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif