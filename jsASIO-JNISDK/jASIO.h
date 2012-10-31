#include "jni.h"
#include "asiosys.h"
#include "asio.h"

#if MAC
//#include <Devices.h>
#include <Timer.h>
//#include <Math64.h>
#else
//#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#endif


jlong getThis(JNIEnv *, jobject obj);

void cb_bufferSwitch(long index, ASIOBool processNow);
ASIOTime* cb_bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
void cb_sampleRateChanged(ASIOSampleRate sRate);
long cb_asioMessage(long selector, long value, void* message, double* opt);

extern JavaVM* asioJVM;
extern bool jvmAttached;
static jobject activeMixer;
