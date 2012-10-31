#include "jasiomixer.h"
#include "jASIO.h"
#include "asio.h"

void cb_bufferSwitch(long index, ASIOBool processNow){       
    ASIOTime* time = new ASIOTime;

	if(ASIOGetSamplePosition(&time->timeInfo.samplePosition, &time->timeInfo.systemTime) == ASE_OK)
		time->timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

    // Call the real callback
    cb_bufferSwitchTimeInfo (time, index, processNow);
}

ASIOTime* cb_bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow){
	JNIEnv* env;

    jint result = asioJVM->GetEnv((void **)&env, JNI_VERSION_1_1);
    if (result == JNI_EDETACHED) {
        result = asioJVM->AttachCurrentThread((void **)&env, NULL);
        jvmAttached = true;
    }

	//asioJVM->AttachCurrentThread((void**)&env, NULL);
	jmethodID jmidBS = env->GetMethodID(env->GetObjectClass(activeMixer), "jBufferSwitchTimeInfo", "(JIZ)V");
	env->CallVoidMethod(activeMixer, jmidBS, (jlong)timeInfo, index, processNow);
	//asioJVM->DetachCurrentThread();
	
	return 0L;
}

void cb_sampleRateChanged(ASIOSampleRate sRate){
	JNIEnv* env;

    jint result = asioJVM->GetEnv((void **)&env, JNI_VERSION_1_1);
    if (result == JNI_EDETACHED) {
        result = asioJVM->AttachCurrentThread((void **)&env, NULL);
        jvmAttached = true;
    }

	//asioJVM->AttachCurrentThread((void**)&env, NULL);
	jclass provider = env->FindClass("com/groovemanager/spi/asio/ASIOMixerProvider");
	jmethodID jmidSRC = env->GetStaticMethodID(provider, "jSampleRateDidChange", "(J)V");
	env->CallStaticVoidMethod(provider, jmidSRC, (jlong)&sRate);
	//asioJVM->DetachCurrentThread();
}

long cb_asioMessage(long selector, long value, void* message, double* opt){
	JNIEnv* env;

    jint envresult = asioJVM->GetEnv((void **)&env, JNI_VERSION_1_1);
    if (envresult == JNI_EDETACHED) {
        envresult = asioJVM->AttachCurrentThread((void **)&env, NULL);
        jvmAttached = true;
    }

	//asioJVM->AttachCurrentThread((void**)&env, NULL);
	jclass provider = env->FindClass("com/groovemanager/spi/asio/ASIOMixerProvider");
	jmethodID jmidAM = env->GetStaticMethodID(provider, "asioMessage", "(II)I");
	long result = env->CallStaticIntMethod(provider, jmidAM, selector, value);
	//asioJVM->DetachCurrentThread();
	return result;
}


extern "C"{

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOMixer_initVM
(JNIEnv *env, jobject obj){
	env->GetJavaVM(&asioJVM);
	activeMixer = env->NewGlobalRef(obj);
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_ASIOMixer_freeVM
(JNIEnv *env, jobject obj){
	env->DeleteGlobalRef(activeMixer);
}

}
