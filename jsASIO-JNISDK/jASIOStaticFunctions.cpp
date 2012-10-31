#include "asiosys.h"
#include "jasiostaticfunctions.h"
#include "jASIO.h"
#include "asio.h"

static ASIOCallbacks jcallbacks;

extern "C"{

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOInit
(JNIEnv *env, jclass c, jlong pointer){
	ASIODriverInfo* info = (ASIODriverInfo*)pointer;
	return ASIOInit(info);
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOExit
(JNIEnv *env, jclass){
	return ASIOExit();
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOStart
(JNIEnv *env, jclass c){
	return ASIOStart();
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOStop
(JNIEnv *, jclass){
    if (jvmAttached) {
      asioJVM->DetachCurrentThread();
    }
    jvmAttached = false;
	return ASIOStop();
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetChannels
(JNIEnv *env, jclass, jintArray channels){
	jint* temp = env->GetIntArrayElements(channels, NULL);
	
	int code = ASIOGetChannels(&temp[0], &temp[1]);
	env->ReleaseIntArrayElements(channels, temp, 0);
	return code;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetLatencies
(JNIEnv *env, jclass, jintArray latencies){
	jint* temp = env->GetIntArrayElements(latencies, NULL);

	int code = ASIOGetLatencies(&temp[0], &temp[1]);
	env->ReleaseIntArrayElements(latencies, temp, 0);
	return code;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetBufferSize
(JNIEnv *env, jclass, jintArray data){
	jint* temp = env->GetIntArrayElements(data, NULL);
	
	int code = ASIOGetBufferSize(&temp[0], &temp[1], &temp[2], &temp[3]);
	env->ReleaseIntArrayElements(data, temp, 0);
	return code;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOCanSampleRate
(JNIEnv *env, jclass, jobject sampleRate){
	return ASIOCanSampleRate(*((ASIOSampleRate*)getThis(env, sampleRate)));
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetSampleRate
(JNIEnv *env, jclass, jobject sampleRate){
	return ASIOGetSampleRate((ASIOSampleRate*) getThis(env, sampleRate));
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOSetSampleRate
(JNIEnv *env, jclass, jobject sampleRate){
	return ASIOSetSampleRate((*(ASIOSampleRate*)getThis(env, sampleRate)));
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetClockSources
(JNIEnv *env, jclass, jlongArray result){
	jlong* pointers = env->GetLongArrayElements(result, NULL);
	long length = env->GetArrayLength(result) - 1;
	long numSources = length;
	ASIOClockSource* sources = new ASIOClockSource[numSources];
    
	jint code = (jint)ASIOGetClockSources(sources, &numSources);
	for(int i = 0; i < numSources; i++){
		pointers[i] = (jlong)&(sources[i]);
	}
	pointers[length] = numSources;
	env->ReleaseLongArrayElements(result, pointers, 0);

	return code;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOSetClockSource
(JNIEnv *, jclass, jint reference){
	return ASIOSetClockSource(reference);
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOGetChannelInfo
(JNIEnv *env, jclass, jlong pointer){
	ASIOChannelInfo* info = (ASIOChannelInfo*)pointer;
	return ASIOGetChannelInfo(info);
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOCreateBuffers
(JNIEnv *env, jclass, jlongArray pointers, jint buffersize){
	long numChannels = env->GetArrayLength(pointers);
	ASIOBufferInfo* bufferInfos = new ASIOBufferInfo[numChannels];
	jlong* elements = env->GetLongArrayElements(pointers, false);
	for(int i = 0; i < numChannels; i++){
		bufferInfos[i] = (*(ASIOBufferInfo*)elements[i]);
	}
	jcallbacks.bufferSwitch = &cb_bufferSwitch;
	jcallbacks.bufferSwitchTimeInfo = &cb_bufferSwitchTimeInfo;
	jcallbacks.sampleRateDidChange = &cb_sampleRateChanged;
	jcallbacks.asioMessage = &cb_asioMessage;
	jint code = ASIOCreateBuffers(bufferInfos, numChannels, buffersize, &jcallbacks);
	for(int i = 0; i < numChannels; i++){
		elements[i] = (jlong)&bufferInfos[i];
	}
	env->ReleaseLongArrayElements(pointers, elements, 0);
	return code;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIODisposeBuffers
(JNIEnv *, jclass){
	return ASIODisposeBuffers();
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOOutputReady
(JNIEnv *, jclass){
	return ASIOOutputReady();
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOControlPanel
(JNIEnv *, jclass){
	return ASIOControlPanel();
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_jASIOFuture
(JNIEnv *, jclass, jint sel, jlong opt){
	return ASIOFuture(sel, (void*)opt);
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioEnableTimeCodeRead
(JNIEnv *, jclass){
	return kAsioEnableTimeCodeRead;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioDisableTimeCodeRead
(JNIEnv *, jclass){
	return kAsioDisableTimeCodeRead;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSetInputMonitor
(JNIEnv *, jclass){
	return kAsioSetInputMonitor;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSelectorSupported
(JNIEnv *, jclass){
	return kAsioSelectorSupported;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioEngineVersion
(JNIEnv *, jclass){
	return kAsioEngineVersion;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioResetRequest
(JNIEnv *, jclass){
	return kAsioResetRequest;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioBufferSizeChange
(JNIEnv *, jclass){
	return kAsioBufferSizeChange;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioResyncRequest
(JNIEnv *, jclass){
	return kAsioResyncRequest;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioLatenciesChanged
(JNIEnv *, jclass){
	return kAsioLatenciesChanged;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSupportsTimeInfo
(JNIEnv *, jclass){
	return kAsioSupportsTimeInfo;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_kAsioSupportsTimeCode
(JNIEnv *, jclass){
	return kAsioSupportsTimeCode;
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_getCallbackPointer
(JNIEnv *, jclass){
	return (jlong)&jcallbacks;
}

JNIEXPORT jobject JNICALL Java_com_groovemanager_spi_asio_ASIOStaticFunctions_createBuffer
(JNIEnv *env, jclass c, jlong infoPointer, jint index, jint capacity){
	ASIOBufferInfo* info = (ASIOBufferInfo*)infoPointer;
	return env->NewDirectByteBuffer(info->buffers[index], capacity);
}

}
