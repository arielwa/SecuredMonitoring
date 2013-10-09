/*
 * weka_jni.c
 *
 *  Created on: 2013.10.07
 *      Author: Tomer Godinger
 */

/* Package: il.ac.bgu.wekawrapper */

#include <fcntl.h>
#include <sys/mman.h>
#include <jni.h>
#include <android/log.h>
#include <errno.h>

#include "weka_jni.h"

void Java_il_ac_bgu_wekawrapper_MainActivity_init(JNIEnv* env, jobject thiz)
{
	// Test method
	int fd = open(DEVICE_NAME, O_RDONLY);
	close(fd);
}

jint Java_il_ac_bgu_wekawrapper_MainActivity_readMemory(JNIEnv* env,
		jobject thiz, jobject buffer, jint bufferSize)
{
	// Get the buffer
	// 	( Taken from: http://stackoverflow.com/questions/11329519/manipulation-of-bytebuffer-from-jni )
    jclass cls = (*env)->GetObjectClass(env, buffer);
    jmethodID mid = (*env)->GetMethodID(env, cls, "limit", "(I)Ljava/nio/Buffer;");
    char *buf = (char*)(*env)->GetDirectBufferAddress(env, buffer);
    jlong capacity = (*env)->GetDirectBufferCapacity(env, buffer);
    if (capacity < bufferSize) bufferSize = capacity;

    // Read the data
    int nBytesRead = 0;
	int fd = open(DEVICE_NAME, O_RDONLY);
	__android_log_print(ANDROID_LOG_DEBUG, "weka_jni", "buf = %p, fd = %d\n", buf, fd);
	if (fd > 0)
	{
		lseek(fd, 0x0035FE08, SEEK_SET);
		nBytesRead = read(fd, buf, bufferSize);
		close(fd);
	}
	else
	{
		__android_log_print(ANDROID_LOG_DEBUG, "weka_jni", "errno = %d", errno);
	}

	// Return the amount of bytes read
	(*env)->CallObjectMethod(env, buffer, mid, nBytesRead);
	return nBytesRead;
}

jobject Java_il_ac_bgu_wekawrapper_MainActivity_getMemoryBuffer(JNIEnv* env,
		jobject thiz, jint bufferSize)
{
	// Map the device to a memory region and return it as a Java byte array
	int fd = open(DEVICE_NAME, O_RDONLY);
	void* pMemoryBuffer = mmap(NULL, bufferSize, PROT_READ, MAP_SHARED, fd, 0);
	return (*env)->NewDirectByteBuffer(env, pMemoryBuffer, bufferSize);
}
