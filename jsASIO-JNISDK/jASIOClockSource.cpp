#include "jasioclocksource.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOClockSource_index
(JNIEnv *env, jobject obj){
	ASIOClockSource* source = (ASIOClockSource*)getThis(env, obj);
	return source->index;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOClockSource_associatedChannel
(JNIEnv *env, jobject obj){
	ASIOClockSource* source = (ASIOClockSource*)getThis(env, obj);
	return source->associatedChannel;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOClockSource_associatedGroup
(JNIEnv *env, jobject obj){
	ASIOClockSource* source = (ASIOClockSource*)getThis(env, obj);
	return source->associatedGroup;
}

JNIEXPORT jboolean JNICALL Java_com_groovemanager_spi_asio_ASIOClockSource_isCurrentSource
(JNIEnv *env, jobject obj){
	ASIOClockSource* source = (ASIOClockSource*)getThis(env, obj);
	return (jboolean) source->isCurrentSource;
}

JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_ASIOClockSource_name
(JNIEnv *env, jobject obj){
	ASIOClockSource* source = (ASIOClockSource*)getThis(env, obj);
	char *name = source->name;
	return env->NewStringUTF(name);
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOClockSource_createClass
(JNIEnv *env, jobject obj){
	ASIOClockSource* source = new ASIOClockSource;
	return (jlong)source;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOClockSource_cleanUp
(JNIEnv *env, jobject obj){
	ASIOClockSource* source = (ASIOClockSource*)getThis(env, obj);
	delete source;
}

}