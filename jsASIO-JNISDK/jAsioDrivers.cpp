#include "jasiodrivers.h"
#include "asiodrivers.h"
#include "jASIO.h"

char** jasiodrivernames;
long jasiodriverlength = 0;

extern "C"{


JNIEXPORT jboolean JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_loadDriver
(JNIEnv *env, jobject obj, jstring s){
	// AsioDrivers-Objekt holen
	AsioDrivers* drivers = (AsioDrivers*)getThis(env, obj);
	
	// String zu char-Array umwandeln
	const char* jname = env->GetStringUTFChars(s, NULL);
	jsize len = env->GetStringUTFLength(s);
	// Von Konstante in Variable kopieren
	char* name = new char[len + 1];
	for(int i = 0; i < len + 1; i++){
		name[i] = jname[i];
	}

	// ASIO-Treiber laden
	jboolean success = (jboolean)drivers->loadDriver(name);

	// char-Arrays wieder freigeben
	env->ReleaseStringUTFChars(s, jname);
	delete name;

	return success;
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_prepareDriverNames
(JNIEnv *env, jobject obj, jint maxDrivers){
	
	// AsioDrivers-Objekt holen
	AsioDrivers* drivers = (AsioDrivers*)getThis(env, obj);

	// Altes Treiber-Array löschen
	for(int i = 0; i < jasiodriverlength; i++){
		delete jasiodrivernames[i];
	}
	delete jasiodrivernames;
	
	// Neues Treiber-Array erzeugen
	jasiodrivernames = new char*[maxDrivers];
	for(int i = 0; i < maxDrivers; i++){
		// ASIO sieht eine String-Länge von 32 für die Treibernamen vor.
		jasiodrivernames[i] = new char[32];
	}

	// ASIO nach Treibernamen fragen
	jasiodriverlength = drivers->getDriverNames(jasiodrivernames, maxDrivers);
	
	// Anzahl der gefundenen Treiber zurückgeben
	return jasiodriverlength;
}

JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_getDriverName
(JNIEnv *env, jobject obj, jint index){
	char* name = jasiodrivernames[index];
	return env->NewStringUTF(name);
}

JNIEXPORT jstring JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_getCurrentDriverName
(JNIEnv *env, jobject obj){
	// AsioDrivers-Objekt holen
	AsioDrivers* drivers = (AsioDrivers*)getThis(env, obj);
	
	// Char-Array erzeugen (Wird später nicht gelöscht, 
	// weil das Java String-Objekt drauf zugreift)
	char* name = new char[32];

	// Treibernamen holen
	drivers->getCurrentDriverName(name);
	return env->NewStringUTF(name);
}

JNIEXPORT jint JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_getCurrentDriverIndex
(JNIEnv *env, jobject obj){
	AsioDrivers* drivers = (AsioDrivers*)getThis(env, obj);
	return drivers->getCurrentDriverIndex();
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_removeCurrentDriver
(JNIEnv *env, jobject obj){
	AsioDrivers* drivers = (AsioDrivers*)getThis(env, obj);
	drivers->removeCurrentDriver();
}

JNIEXPORT jlong JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_createClass
(JNIEnv *env, jobject obj){
	AsioDrivers* drivers = new AsioDrivers();
	return (jlong)drivers;
}

JNIEXPORT void JNICALL Java_com_groovemanager_spi_asio_AsioDrivers_cleanUp
(JNIEnv *env, jobject obj){
	AsioDrivers* drivers = (AsioDrivers*)getThis(env, obj);
	delete drivers;
}

}