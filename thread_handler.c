#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

#include "linkedlist_handler.h"
#include "thread_handler.h"

timer_t td_ret_timer;
struct itimerspec td_ret_timer_value;

pthread_mutex_t td_set_mutex;

struct td_freq_ctl {
	const char* filepath;
	const char* low;
	const char* high;
};

struct td_freq_ctl* td_freq_ctls;
int td_freq_ctls_c;

void td_state_set(int state) {
	pthread_mutex_lock(&td_set_mutex);

	for (int i = 0; i < td_freq_ctls_c; i++) {
		FILE* freq_ctl_handle = fopen(td_freq_ctls[i].filepath, "w");
		fprintf(freq_ctl_handle, "%s", (state == 0) ? (td_freq_ctls[i].low) : (td_freq_ctls[i].high));
		fclose(freq_ctl_handle);
	}

	if (state != 0) {
		timer_settime(td_ret_timer, 0, &td_ret_timer_value, NULL);
	}

	pthread_mutex_unlock(&td_set_mutex);
}

void td_state_timeout(union sigval sv) {
	td_state_set(0);
}

void* td_thread_callback(void* var) {
	FILE* file = ((struct ll_thread_obj*)var)->file;
	while (1) {
		fgetc(file);
		td_state_set(1);
		usleep(100000);
		__fpurge(file);
	}
	return NULL;
}

void td_thread_create(struct ll_thread_obj* thread_obj) {
	thread_obj->file = fopen(thread_obj->filepath, "r");
	pthread_create(&thread_obj->thread, NULL, td_thread_callback, (void*)thread_obj);
}

void td_thread_delete(struct ll_thread_obj* thread_obj) {
	pthread_mutex_lock(&td_set_mutex);
	pthread_cancel(thread_obj->thread);
	pthread_mutex_unlock(&td_set_mutex);
	fclose(thread_obj->file);
}

void td_init() {
	pthread_mutex_init(&td_set_mutex, NULL);

	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = td_state_timeout;
	sev.sigev_notify_attributes = NULL;
	timer_create(CLOCK_MONOTONIC, &sev, &td_ret_timer);
	td_ret_timer_value.it_value.tv_sec = 1;
	td_ret_timer_value.it_value.tv_nsec = 0;
}

void td_freq_ctls_init(int argc, const char** argv) {
	td_freq_ctls_c = (argc - 1) / 3;
	td_freq_ctls = malloc(sizeof(struct td_freq_ctl) * td_freq_ctls_c);
	for (int i = 0; i < td_freq_ctls_c; i++) {
		td_freq_ctls[i].filepath = argv[(i * 3) + 1];
		td_freq_ctls[i].low = argv[(i * 3) + 2];
		td_freq_ctls[i].high = argv[(i * 3) + 3];
	}
}
