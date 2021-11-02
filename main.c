#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "main.h"
#include "udev_handler.h"
#include "linkedlist_handler.h"
#include "thread_handler.h"

struct ll_thread_obj* head = NULL;

/* Add an input source to the list */
void input_add(const char* filepath) {
	UNUSED(ll_add(&head, td_thread_create(filepath)));
}

/* Remove an input source from the list */
void input_del(const char* filepath) {
	td_thread_delete(ll_find_and_remove(&head, filepath));
}

/* Handle udev events to add or remove input devices from the list */
void input_mod(const char* action, const char* filepath) {
	if (strcmp(action, "add") == 0) {
		input_add(filepath);
	} else if (strcmp(action, "remove") == 0) {
		input_del(filepath);
	}
}

/* Gracefully stop the program when a configured signal is received */
void sig_handler(int signum) {
	UNUSED(signum);

	/* Could still use some nicer code */
	while (head != NULL) {
		input_del(head->filepath);
	}
	td_state_set(0);

	exit(0);
}

int main(int argc, const char** argv) {
	if (argc % 3 != 1 || argc == 1) {
		fprintf(stderr, "A multiple of 3 arguments is required\n");
		return 1;
	}

	signal(SIGHUP, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTERM, sig_handler);

	ud_find_devices(&input_add);
	td_init();
	td_freq_ctls_init(argc, argv);

	ud_monitor_devices(&input_mod);
	return 0;
}
