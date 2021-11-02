#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

#include "main.h"
#include "linkedlist_handler.h"
#include "thread_handler.h"

/* Reset timer details */
timer_t td_ret_timer;
struct itimerspec td_ret_timer_value;

pthread_mutex_t td_set_mutex;

struct td_freq_ctl {
	const char* filepath; /* File to write to */
	const char* low; /* Value to write on reset */
	const char* high; /* Value to write on boost */
};

struct td_freq_ctl* td_freq_ctls;
int td_freq_ctls_c;

/* Set boost state by writing to all configured frequency control files */
void td_state_set(int state) {
	/* Get a mutex lock, many threads will be using this simultaneously */
	pthread_mutex_lock(&td_set_mutex);

	/* Open each file, write the correct value to it and close it */
	for (int i = 0; i < td_freq_ctls_c; i++) {
		FILE* freq_ctl_handle = fopen(td_freq_ctls[i].filepath, "w");
		fprintf(freq_ctl_handle, "%s", (state == 0) ? (td_freq_ctls[i].low) : (td_freq_ctls[i].high));
		fclose(freq_ctl_handle);
	}

	/* Start the reset timer if we're beginning a boost */
	if (state != 0) {
		timer_settime(td_ret_timer, 0, &td_ret_timer_value, NULL);
	}

	pthread_mutex_unlock(&td_set_mutex);
}

/* Reset timer callback */
void td_state_timeout(union sigval sv) {
	UNUSED(sv);
	td_state_set(0);
}

/* Input watching thread main function */
void* td_thread_callback(void* var) {
	/* Constantly try to read from configured file, when read succeeds start a boost */
	FILE* file = ((struct ll_thread_obj*)var)->file;

	while (1) {
		fgetc(file);
		td_state_set(1);
		usleep(100000);
		__fpurge(file); /* This is horrible but reduces CPU load and doesn't seem to have a good alternative */
	}

	return NULL;
}

/* Create a new thread */
struct ll_thread_obj* td_thread_create(const char* filepath) {
	struct ll_thread_obj* thread_obj = malloc(sizeof(struct ll_thread_obj));

	thread_obj->filepath = malloc(strlen(filepath) + 1);
	strcpy(thread_obj->filepath, filepath);

	thread_obj->file = fopen(thread_obj->filepath, "r");
	pthread_create(&thread_obj->thread, NULL, td_thread_callback, (void*)thread_obj);

	return thread_obj;
}

/* Delete a thread */
void td_thread_delete(struct ll_thread_obj* thread_obj) {
	if (thread_obj == NULL) {
		return;
	}

	/* Get a mutex lock so we don't potentially kill the thread while it's doing something important */
	pthread_mutex_lock(&td_set_mutex);

	pthread_cancel(thread_obj->thread);
	fclose(thread_obj->file);

	pthread_mutex_unlock(&td_set_mutex);

	free(thread_obj->filepath);
	free(thread_obj);
}

/* Setup mutex and timer */
void td_init(void) {
	pthread_mutex_init(&td_set_mutex, NULL);

	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = td_state_timeout;
	sev.sigev_notify_attributes = NULL;

	timer_create(CLOCK_MONOTONIC, &sev, &td_ret_timer);
	td_ret_timer_value.it_value.tv_sec = 1;
	td_ret_timer_value.it_value.tv_nsec = 0;
}

/* Set up frequency control files */
void td_freq_ctls_init(int argc, const char** argv) {
	td_freq_ctls_c = (argc - 1) / 3;
	td_freq_ctls = malloc(sizeof(struct td_freq_ctl) * td_freq_ctls_c);

	for (int i = 0; i < td_freq_ctls_c; i++) {
		td_freq_ctls[i].filepath = argv[(i * 3) + 1];
		td_freq_ctls[i].low = argv[(i * 3) + 2];
		td_freq_ctls[i].high = argv[(i * 3) + 3];
	}
}
