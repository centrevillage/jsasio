#include "jasiotimestamp.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOTimeStamp_createClass
(JNIEnv *env, jobject obj){
	ASIOTimeStamp* stamp = new ASIOTimeStamp;
	return (jlong)stamp;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOTimeStamp_cleanUp
(JNIEnv *env, jobject obj){
	ASIOTimeStamp* stamp = (ASIOTimeStamp*)getThis(env, obj);
	delete stamp;
}
JNIEXPORT jdouble JNICALL Java_com_groovemanager_spi_asio_ASIOTimeStamp_doubleValue
(JNIEnv *env, jobject obj){
	ASIOTimeStamp* stamp = (ASIOTimeStamp*)getThis(env, obj);
	static const double twoRaisedTo32 = 4294967296.;
	double r = ((double)stamp->hi * twoRaisedTo32 + (double)stamp->lo);
	return r;
}

}