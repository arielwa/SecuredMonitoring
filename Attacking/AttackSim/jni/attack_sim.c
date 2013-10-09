#include <unistd.h>
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "attack_sim.h"

void Java_com_example_attacksim_AttackSim_changeUid(JNIEnv* env, jobject thiz, jint uid) {
	int fd = open("/dev/ninja", 0);
	ioctl(fd, IOCTL_UID, uid);

	int sockfd;
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	ioctl(fd, IOCTL_LOG_STRING, "hallo ");
	ioctl(fd, IOCTL_LOG_INT, sockfd);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(6666);
	inet_pton(AF_INET, "10.0.2.2", &serv_addr.sin_addr);
	ioctl(fd, IOCTL_LOG_INT, connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));

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
