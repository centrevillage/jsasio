#include "jasioerror.h"
#include "asio.h"
#include "jASIO.h"

extern "C"{

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1OK
(JNIEnv *, jclass){
	return ASE_OK;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1SUCCESS
(JNIEnv *, jclass){
	return ASE_SUCCESS;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1NotPresent
(JNIEnv *, jclass){
	return ASE_NotPresent;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1HWMalfunction
(JNIEnv *, jclass){
	return ASE_HWMalfunction;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1InvalidParameter
(JNIEnv *, jclass){
	return ASE_InvalidParameter;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1InvalidMode
(JNIEnv *, jclass){
	return ASE_InvalidMode;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1SPNotAdvancing
(JNIEnv *, jclass){
	return ASE_SPNotAdvancing;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1NoClock
(JNIEnv *, jclass){
	return ASE_NoClock;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_ASIOError_ASE_1NoMemory
(JNIEnv *, jclass){
	return ASE_NoMemory;
}

}