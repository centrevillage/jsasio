#include "jasiotimecode.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_createClass
(JNIEnv *env, jobject obj){
	ASIOTimeCode* tc = new ASIOTimeCode;
	return (jlong)tc;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_cleanUp
(JNIEnv *env, jobject obj){
	ASIOTimeCode* tc = (ASIOTimeCode*)getThis(env, obj);
	delete tc;
}

JNIEXPORT jdouble JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_speed
(JNIEnv *env, jobject obj){
	ASIOTimeCode* tc = (ASIOTimeCode*)getThis(env, obj);
	return tc->speed;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_jtimeCodeSamples
(JNIEnv *env, jobject obj){
	ASIOTimeCode* tc = (ASIOTimeCode*)getThis(env, obj);
	return (jlong)(&tc->timeCodeSamples);
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_flags
(JNIEnv *env, jobject obj){
	ASIOTimeCode* tc = (ASIOTimeCode*)getThis(env, obj);
	return tc->flags;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_kTcValid
(JNIEnv *, jclass){
	return kTcValid;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_kTcRunning
(JNIEnv *, jclass){
	return kTcRunning;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_kTcReverse
(JNIEnv *, jclass){
	return kTcReverse;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_kTcOnspeed
(JNIEnv *, jclass){
	return kTcOnspeed;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_kTcStill
(JNIEnv *, jclass){
	return kTcStill;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeCode_kTcSpeedValid
(JNIEnv *, jclass){
	return kTcSpeedValid;
}

}