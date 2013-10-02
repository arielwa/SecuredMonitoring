#include <unistd.h>
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <fcntl.h>

#include "attack_sim.h"

void Java_com_example_attacksim_AttackSim_changeUid(JNIEnv* env, jobject thiz, jint uid) {
	int fd = open("/dev/ninja", 0);
	ioctl(fd, IOCTL_UID, uid);
	close(fd);
}

void Java_com_example_attacksim_AttackSim_hookSyscall(JNIEnv* env, jobject thiz, jint syscall) {
	int fd = open("/dev/ninja", 0);
	ioctl(fd, IOCTL_SYS_CALL, syscall);
	close(fd);
}

void Java_com_example_attacksim_AttackSim_changeContextManager(JNIEnv* env, jobject thiz) {
	if (fork() == 0)
		svcmgr_main();
}
