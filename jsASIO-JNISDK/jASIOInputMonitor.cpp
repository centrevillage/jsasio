#include "jasioinputmonitor.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_input
(JNIEnv *env, jobject obj){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	return monitor->input;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_output
(JNIEnv *env, jobject obj){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	return monitor->output;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_gain
(JNIEnv *env, jobject obj){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	return monitor->gain;
}

JNIEXPORT jboolean JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_state
(JNIEnv *env, jobject obj){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	return monitor->state;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_pan
(JNIEnv *env, jobject obj){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	return monitor->pan;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_setInput
(JNIEnv *env, jobject obj, jint input){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	monitor->input = input;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_setOutput
(JNIEnv *env, jobject obj, jint output){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	monitor->output = output;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_setGain
(JNIEnv *env, jobject obj, jint gain){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	monitor->gain = gain;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_setPan
(JNIEnv *env, jobject obj, jint pan){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	monitor->pan = pan;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_setState
(JNIEnv *env, jobject obj, jboolean state){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	monitor->state = state;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_createClass
(JNIEnv *env, jobject obj){
	ASIOInputMonitor* monitor = new ASIOInputMonitor;
	return (jlong)monitor;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOInputMonitor_cleanUp
(JNIEnv *env, jobject obj){
	ASIOInputMonitor* monitor = (ASIOInputMonitor*)getThis(env, obj);
	delete monitor;
}

}