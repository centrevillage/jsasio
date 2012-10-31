#include "jasiosamples.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOSamples_createClass
(JNIEnv *env, jobject obj){
	ASIOSamples* samples = new ASIOSamples;
	return (jlong)samples;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOSamples_cleanUp
(JNIEnv *env, jobject obj){
	ASIOSamples* samples = (ASIOSamples*)getThis(env, obj);
	delete samples;
}

JNIEXPORT jdouble JNICALL Java_com_groovemanager_spi_asio_ASIOSamples_doubleValue
(JNIEnv *env, jobject obj){
	ASIOSamples* samples = (ASIOSamples*)getThis(env, obj);
	static const double twoRaisedTo32 = 4294967296.;
	double r = ((double)samples->hi * twoRaisedTo32 + (double)samples->lo);
	return r;
}

}
