#include "jasiobufferinfo.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jboolean JNICALL Java_com_groovemanager_spi_asio_ASIOBufferInfo_isInput
(JNIEnv *env, jobject obj){
	ASIOBufferInfo* info = (ASIOBufferInfo*)getThis(env, obj);
	return info->isInput;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOBufferInfo_setIsInput
(JNIEnv *env, jobject obj, jboolean isInput){
	ASIOBufferInfo* info = (ASIOBufferInfo*)getThis(env, obj);
	info->isInput = (ASIOBool)isInput;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOBufferInfo_channelNum
(JNIEnv *env, jobject obj){
	ASIOBufferInfo* info = (ASIOBufferInfo*)getThis(env, obj);
	return info->channelNum;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOBufferInfo_setChannelNum
(JNIEnv *env, jobject obj, jint num){
	ASIOBufferInfo* info = (ASIOBufferInfo*)getThis(env, obj);
	info->channelNum = num;
}

JNIEXPORT jlongArray JNICALL Java_com_groovemanager_spi_asio_ASIOBufferInfo_buffers
(JNIEnv *env, jobject obj){
	ASIOBufferInfo* info = (ASIOBufferInfo*)getThis(env, obj);
	jlongArray result = env->NewLongArray(2);
	jlong* jbuffers = env->GetLongArrayElements(result, NULL);
	
	jbuffers[0] = (jlong)info->buffers[0];
	jbuffers[1] = (jlong)info->buffers[1];
	
	env->ReleaseLongArrayElements(result, jbuffers, 0);

	return result;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOBufferInfo_createClass
(JNIEnv *env, jobject obj){
	ASIOBufferInfo* info = new ASIOBufferInfo;
	info->buffers[0] = info->buffers[1] = 0;
	return (jlong)info;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOBufferInfo_cleanUp
(JNIEnv *env, jobject obj){
	ASIOBufferInfo* info = (ASIOBufferInfo*)getThis(env, obj);
	delete info;
}

}