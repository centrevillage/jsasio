#include "jasiotime.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTime_jTimeInfo
(JNIEnv *env, jobject obj){
	ASIOTime* t = (ASIOTime*)getThis(env, obj);
	return (jlong)(&(t->timeInfo));
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTime_jTimeCode
(JNIEnv *env, jobject obj){
	ASIOTime* t = (ASIOTime*)getThis(env, obj);
	return (jlong)(&(t->timeCode));
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTime_createClass
(JNIEnv *env, jobject obj){
	ASIOTime* t = new ASIOTime;
	return (jlong)t;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOTime_cleanUp
(JNIEnv *env, jobject obj){
	ASIOTime* t = (ASIOTime*)getThis(env, obj);
	delete t;
}

}