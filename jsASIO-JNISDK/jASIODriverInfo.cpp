#include "jasiodriverinfo.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_setAsioVersion
(JNIEnv * env, jobject obj, jint version){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	info->asioVersion = version;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_setSysRef
(JNIEnv *env, jobject obj, jlong sysRef){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	info->sysRef = (void*)sysRef;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_asioVersion
(JNIEnv *env, jobject obj){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	return info->asioVersion;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_driverVersion
(JNIEnv *env, jobject obj){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	return info->driverVersion;
}

JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_name
(JNIEnv *env, jobject obj){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	return env->NewStringUTF(info->name);
}

JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_errorMessage
(JNIEnv *env, jobject obj){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	return env->NewStringUTF(info->errorMessage);
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_sysRef
(JNIEnv *env, jobject obj){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	return (jlong)info->sysRef;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_createClass
(JNIEnv *env, jobject obj){
	ASIODriverInfo* info = new ASIODriverInfo;
	return (jlong)info;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIODriverInfo_cleanUp
(JNIEnv *env, jobject obj){
	ASIODriverInfo* info = (ASIODriverInfo*)getThis(env, obj);
	delete info;
}

}