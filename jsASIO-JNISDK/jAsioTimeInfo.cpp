#include "jasiotimeinfo.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_createClass
(JNIEnv *env, jobject obj){
	AsioTimeInfo* info = new AsioTimeInfo;
	return (jlong)info;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_cleanUp
(JNIEnv *env, jobject obj){
	AsioTimeInfo* info = (AsioTimeInfo*)getThis(env, obj);
	delete(info);
}

JNIEXPORT jdouble JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_speed
(JNIEnv *env, jobject obj){
	AsioTimeInfo* info = (AsioTimeInfo*)getThis(env, obj);
	return info->speed;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_jsystemTime
(JNIEnv *env, jobject obj){
	AsioTimeInfo* info = (AsioTimeInfo*)getThis(env, obj);
	return (jlong)(&info->systemTime);
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_jsamplePosition
(JNIEnv *env, jobject obj){
	AsioTimeInfo* info = (AsioTimeInfo*)getThis(env, obj);
	return (jlong)(&info->samplePosition);
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_jsampleRate
(JNIEnv *env, jobject obj){
	AsioTimeInfo* info = (AsioTimeInfo*)getThis(env, obj);
	return (jlong)(&info->sampleRate);
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_flags
(JNIEnv *env, jobject obj){
	AsioTimeInfo* info = (AsioTimeInfo*)getThis(env, obj);
	return info->flags;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_kSystemTimeValid
(JNIEnv *, jclass){
	return kSystemTimeValid;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_kSamplePositionValid
(JNIEnv *, jclass){
	return kSamplePositionValid;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_kSampleRateValid
(JNIEnv *, jclass){
	return kSampleRateValid;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_kSpeedValid
(JNIEnv *, jclass){
	return kSpeedValid;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_kSampleRateChanged
(JNIEnv *, jclass){
	return kSampleRateChanged;
}
JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioTimeInfo_kClockSourceChanged
(JNIEnv *, jclass){
	return kClockSourceChanged;
}

}