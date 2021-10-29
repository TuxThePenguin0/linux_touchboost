#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "udev_handler.h"
#include "linkedlist_handler.h"
#include "thread_handler.h"

struct ll_thread_obj* head = NULL;

void input_add(const char* filepath) {
	struct ll_thread_obj* newthread = malloc(sizeof(struct ll_thread_obj));
	newthread->filepath = malloc(strlen(filepath) + 1);
	strcpy(newthread->filepath, filepath);
	td_thread_create(newthread);
	head = ll_add(head, newthread);
}

void input_del(const char* filepath) {
	head = ll_find_and_remove(head, filepath);
}

void input_mod(const char* action, const char* filepath) {
	if (strcmp(action, "add") == 0) {
		input_add(filepath);
	} else if (strcmp(action, "remove") == 0) {
		input_del(filepath);
	}
}

int main(int argc, const char** argv) {
	if (argc % 3 != 1 || argc == 1) {
		fprintf(stderr, "A multiple of 3 arguments is required\n");
		return 1;
	}

	ud_find_devices(&input_add);
	td_init();
	td_freq_ctls_init(argc, argv);

	ud_monitor_devices(&input_mod);
	return 0;
}
