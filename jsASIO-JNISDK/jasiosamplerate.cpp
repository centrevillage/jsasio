#include "jasiosamplerate.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOSampleRate_createClass
(JNIEnv *, jobject){
	ASIOSampleRate* rate = new ASIOSampleRate;
	return(jlong)rate;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOSampleRate_cleanUp
(JNIEnv *env, jobject obj){
	ASIOSampleRate* rate = (ASIOSampleRate*)getThis(env, obj);
	delete rate;
}

JNIEXPORT jdouble JNICALL Java_com_groovemanager_spi_asio_ASIOSampleRate_doubleValue
(JNIEnv *env, jobject obj){
	ASIOSampleRate* rate = (ASIOSampleRate*)getThis(env, obj);
#if IEEE754_64FLOAT
	return *((jdouble*)rate);
#else
	jobject buffer = env->NewDirectByteBuffer((void*)rate->ieee, 8);
	jmethodID mid = env->GetStaticMethodID(env->GetObjectClass(obj), "convertToDouble", "(Ljava/nio/ByteBuffer;)D");
	return env->CallStaticDoubleMethod(env->GetObjectClass(obj), mid, buffer);
#endif
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOSampleRate_setValue
(JNIEnv *env, jobject obj, jdouble value){
	ASIOSampleRate* rate = (ASIOSampleRate*)getThis(env, obj);
#if IEEE754_64FLOAT
	*rate = *(ASIOSampleRate*)&value;
#else
	jobject buffer = env->NewDirectByteBuffer((void*)rate->ieee, 8);
	jmethodID mid = env->GetStaticMethodID(env->GetObjectClass(obj), "convertBack", "(Ljava/nio/ByteBuffer;D)V");
	return env->CallStaticVoidMethod(env->GetObjectClass(obj), mid, buffer, value);
#endif
}

}