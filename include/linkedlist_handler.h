#ifndef TOUCHBOOST_LL
#define TOUCHBOOST_LL

#include <pthread.h>
#include <stdio.h>

struct ll_thread_obj {
	char* filepath;
	FILE* file;
	pthread_t thread;
	struct ll_thread_obj* next;
};

struct ll_thread_obj* ll_add(struct ll_thread_obj** head, struct ll_thread_obj* newitem);
struct ll_thread_obj* ll_find_and_remove(struct ll_thread_obj** head, const char* filepath);
struct ll_thread_obj* ll_remove(struct ll_thread_obj** obj);

#endif
