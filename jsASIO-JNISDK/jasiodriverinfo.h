/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_groovemanager_spi_asio_ASIODriverInfo */

#ifndef _Included_com_groovemanager_spi_asio_ASIODriverInfo
#define _Included_com_groovemanager_spi_asio_ASIODriverInfo
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    setAsioVersion
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_setAsioVersion
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    setSysRef
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_setSysRef
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    asioVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_asioVersion
  (JNIEnv *, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    driverVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_driverVersion
  (JNIEnv *, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    name
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_name
  (JNIEnv *, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    errorMessage
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_errorMessage
  (JNIEnv *, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    sysRef
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_sysRef
  (JNIEnv *, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    createClass
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_createClass
  (JNIEnv *, jobject);

/*
 * Class:     com_groovemanager_spi_asio_ASIODriverInfo
 * Method:    cleanUp
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_cleanUp
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif