/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_groovemanager_spi_asio_ASIOStaticFunctions */

#ifndef _Included_com_groovemanager_spi_asio_ASIOStaticFunctions
#define _Included_com_groovemanager_spi_asio_ASIOStaticFunctions
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOInit
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOInit
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOExit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOExit
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOStart
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOStart
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOStop
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOStop
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOGetChannels
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetChannels
  (JNIEnv *, jclass, jintArray);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOGetLatencies
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetLatencies
  (JNIEnv *, jclass, jintArray);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOGetBufferSize
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetBufferSize
  (JNIEnv *, jclass, jintArray);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOCanSampleRate
 * Signature: (Lcom/groovemanager/spi/asio/ASIOSampleRate;)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOCanSampleRate
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOGetSampleRate
 * Signature: (Lcom/groovemanager/spi/asio/ASIOSampleRate;)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetSampleRate
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOSetSampleRate
 * Signature: (Lcom/groovemanager/spi/asio/ASIOSampleRate;)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOSetSampleRate
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOGetClockSources
 * Signature: ([J)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetClockSources
  (JNIEnv *, jclass, jlongArray);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOSetClockSource
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOSetClockSource
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOGetSamplePosition
 * Signature: (Lcom/groovemanager/spi/asio/ASIOSamples;Lcom/groovemanager/spi/asio/ASIOTimeStamp;)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetSamplePosition
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOGetChannelInfo
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetChannelInfo
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOCreateBuffers
 * Signature: ([JI)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOCreateBuffers
  (JNIEnv *, jclass, jlongArray, jint);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIODisposeBuffers
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIODisposeBuffers
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOOutputReady
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOOutputReady
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOControlPanel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOControlPanel
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jASIOFuture
 * Signature: (IJ)I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOFuture
  (JNIEnv *, jclass, jint, jlong);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioEnableTimeCodeRead
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioEnableTimeCodeRead
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioDisableTimeCodeRead
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioDisableTimeCodeRead
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioSetInputMonitor
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSetInputMonitor
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    jgetSystemTime
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jgetSystemTime
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    createBuffer
 * Signature: (JII)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_createBuffer
  (JNIEnv *, jclass, jlong, jint, jint);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioSelectorSupported
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSelectorSupported
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioEngineVersion
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioEngineVersion
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioResetRequest
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioResetRequest
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioBufferSizeChange
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioBufferSizeChange
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioResyncRequest
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioResyncRequest
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioLatenciesChanged
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioLatenciesChanged
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioSupportsTimeInfo
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSupportsTimeInfo
  (JNIEnv *, jclass);

/*
 * Class:     com_groovemanager_spi_asio_ASIOStaticFunctions
 * Method:    kAsioSupportsTimeCode
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSupportsTimeCode
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
