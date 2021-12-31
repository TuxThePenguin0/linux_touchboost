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

const char* helpmsg =
"\033[0mtouchboost version "TOUCHBOOST_VERSION"\n"
"A clone of the Android touchboost feature for Linux devices\n\n"
"\033[1;31mIf possible please run this program using your service manager\033[0m\n\n"
"This version of the touchboost utility takes arguments in groups of three:\n"
" - Control\n"
" - Idle state\n"
" - Boost state\n\n"
"The control argument is the path to a file that touchboost will write to when\nchanging states.\n"
"The idle and boost states are what data to write to the file when idling and\nboosting respectively.\n\n"
"touchboost comes with a helper script that can automatically detect frequency\ncontrol devices and what data should be written to them.\n\n"
"The easiest way to run touchboost with this script is:\n"
"   \033[0;37mtouchboost `touchboost_detect`\033[0m\n\n";

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

	/* Stop each thread and remove from list */
	while (head != NULL) {
		td_thread_delete(ll_remove(&head));
	}

	/* Reset state and exit program */
	td_state_set(0);
	exit(0);
}

int main(int argc, const char** argv) {
	if (argc % 3 != 1 || argc == 1) {
		fprintf(stdout, helpmsg);
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
