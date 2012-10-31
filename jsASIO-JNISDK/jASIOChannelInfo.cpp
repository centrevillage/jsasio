#include "jasiochannelinfo.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_channel
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	return info->channel;
}

JNIEXPORT jboolean JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_isInput
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	return (jboolean)info->isInput;
}
JNIEXPORT jboolean JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_isActive
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	return (jboolean)info->isActive;
}
JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_channelGroup
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	return info->channelGroup;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_type
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	return info->type;
}

JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_name
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	return env->NewStringUTF(info->name);
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_setChannel
(JNIEnv *env, jobject obj, jint num){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	info->channel = num;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_setIsInput
(JNIEnv *env, jobject obj, jboolean isInput){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	info->isInput = isInput;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_createClass
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = new ASIOChannelInfo;
	return (jlong)info;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOChannelInfo_cleanUp
(JNIEnv *env, jobject obj){
	ASIOChannelInfo* info = (ASIOChannelInfo*)getThis(env, obj);
	delete info;
}

}