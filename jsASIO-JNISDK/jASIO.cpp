#include "jni.h"
#include "jASIO.h"

// Get a native instance out of a Java NativeClass instance
jlong getThis(JNIEnv *env, jobject obj){
	jfieldID id = env->GetFieldID(env->GetObjectClass(obj), "pointer", "J");
	return env->GetLongField(obj, id);
}

JavaVM* asioJVM;
bool jvmAttached;
