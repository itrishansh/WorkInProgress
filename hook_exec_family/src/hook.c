#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <stdlib.h>

typedef int (*execlPtrType)(const char *path, const char *arg, ...);
typedef int (*execlpPtrType)(const char *path, const char *arg, ...);
typedef int (*execlePtrType)(const char *path, const char *arg, ...);
typedef int (*execvPtrType)(const char *path, char *const argv[]);
typedef int (*execvpPtrType)(const char *path, char *const argv[]);
typedef int (*execvpePtrType)(const char *file, char *const argv[], char *const envp[]);
typedef int (*execvePtrType)(const char *filename, char *const argv[], char *const envp[]);

void *libcHandle;
execlPtrType execlPtr;
execlpPtrType execlpPtr;
execlePtrType execlePtr;
execvPtrType execvPtr;
execvpPtrType execvpPtr;
execvpePtrType execvpePtr;
execvePtrType execvePtr;

void initFptr () {
	libcHandle = dlopen("libc.so.6", RTLD_LAZY);
	if (libcHandle) {
		execlPtr = dlsym(libcHandle, "execl");
		execlpPtr = dlsym(libcHandle, "execlp");
		execlePtr = dlsym(libcHandle, "execle");
		
		if (!execlPtr || !execlpPtr || !execlePtr) {
			printf("%s:%d ERROR: execlPtr %p execlpPtr %p execlePtr %p\n", __func__, __LINE__,
					execlPtr, execlpPtr, execlePtr);
		}

		execvPtr = dlsym(libcHandle, "execv");
		execvpPtr = dlsym(libcHandle, "execvp");
		execvpePtr = dlsym(libcHandle, "execvpe");

		if (!execvPtr || !execvpPtr || !execvpePtr) {
			printf("%s:%d ERROR: execvPtr %p execvpPtr %p execvpePtr %p\n", __func__, __LINE__,
					execvPtr, execvpPtr, execvpePtr);
		}

		execvePtr = dlsym(libcHandle, "execve");

		if (!execvePtr) {
			printf("%s:%d ERROR: execvePtr %p\n", __func__, __LINE__, execvePtr);
		}

		return;
	}
	printf("%s:%d %s failed to dlopen libc.so\n", __FILE__, __LINE__, __func__);
}

void __attribute__((constructor)) ctor() {
	printf("%s:%d %s called\n", __FILE__, __LINE__, __func__);
	initFptr();
}

void __attribute__((destructor)) dtor(){
	//printf("%s:%d %s called\n", __FILE__, __LINE__, __func__);
}

void logPath(const char *path) {
	printf("path: %s\n", path);
}

void logArgList(const char *tag, char *const argv[]) {
	char *const *ptr = &argv[0];
	while(*ptr) {
		printf("%s %s", tag, *ptr);
		ptr++;
	}
	printf("\n");
}

void logParamsList(const char *argList[1024], const char *path, const char *arg, va_list args) {
	//va_list args;
	const char *ptr;
	size_t i = 0;
	printf("path: %s, args: %s", path, arg);
	argList[i++] = arg;
	//va_start(args, arg);
	ptr = va_arg(args, const char *);
	argList[i++] = ptr;
	while(ptr) {
		printf(" %s", ptr);
		ptr = va_arg(args, const char *);
		argList[i++] = ptr;
	}
	printf("\n");
	//va_end(args);
}

int execl(const char *path, const char *arg, ...) {
	int rc, i;
	const char *ptr;
	va_list args;
	const char *argList[1024];
	va_start(args, arg);
	logParamsList(argList, path, arg, args);
	va_end(args);
/*
	while(argList[i]) {
		printf("%2d %s\n", i, argList[i]);
		i++;
	}
*/
	rc = execvPtr(path, (char *const *)argList);
	return rc;
}

int execlp(const char *path, const char *arg, ...) {
	int rc, i;
	const char *ptr;
	va_list args;
	const char *argList[1024];
	va_start(args, arg);
	logParamsList(argList, path, arg, args);
	va_end(args);
/*
	while(argList[i]) {
		printf("%2d %s\n", i, argList[i]);
		i++;
	}
*/
	rc = execvpPtr(path, (char *const *)argList);
	return rc;
}

int execle(const char *path, const char *arg, ...){
	printf("Hook not implemented for %s\n", __func__);
	exit(1);
}

int execv(const char *path, char *const argv[]) {
	int rc;
	logPath(path);
	logArgList("args: ", argv);
	rc = execvPtr(path, argv);
	return rc;
}

int execvp(const char *path, char *const argv[]) {
	int rc;
	logPath(path);
	logArgList("args: ", argv);
	rc = execvpPtr(path, argv);
	return rc;
}

int execvpe(const char *file, char *const argv[], char *const envp[]) {
	printf("Hook not implemented for execvpe\n");
	exit(1);
}

int execve(const char *path, char *const argv[], char *const envp[]) {
	int rc;
	logPath(path);
	logArgList("args: ", argv);
	logArgList("env: ", envp);
	rc = execvePtr(path, argv, envp);
	return rc;
}
