#include "jasiodataline.h"
#include "asio.h"
#include "jASIO.h"
#include <stdio.h>

extern "C"{

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIODataLine_deliverData
(JNIEnv *env, jobject obj, jlong address, jbyteArray data){
	jsize len = env->GetArrayLength(data);
	jbyte* jdata = env->GetByteArrayElements(data, NULL);
	char* target = (char*)address;
	for(int i = 0; i < len; i++){
		target[i] = jdata[i];
	}
	env->ReleaseByteArrayElements(data, jdata, 0);
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_ASIODataLine_getAddress
(JNIEnv *env, jobject obj, jobject buff){
	return (jlong)env->GetDirectBufferAddress(buff);
}

}