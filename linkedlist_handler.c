#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "linkedlist_handler.h"
#include "thread_handler.h"

struct ll_thread_obj* ll_add(struct ll_thread_obj* head, struct ll_thread_obj* newthread) {
	newthread->next = NULL;

	if (head == NULL) {
		return newthread;
	}
	struct ll_thread_obj* curr = head;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = newthread;
	return head;
}

struct ll_thread_obj* ll_find_and_remove(struct ll_thread_obj* head, const char* filepath) {
	if (head == NULL) {
		return NULL;
	}

	if (strcmp(head->filepath, filepath) == 0) {
		struct ll_thread_obj* tmp = head->next;
		td_thread_delete(head);
		free(head->filepath);
		free(head);
		return tmp;
	}

	struct ll_thread_obj* curr = head;
	while (curr->next != NULL) {
		if (strcmp(curr->next->filepath, filepath) == 0) {
			struct ll_thread_obj* tmp = curr->next->next;
			td_thread_delete(curr->next);
			free(curr->next->filepath);
			free(curr->next);
			curr->next = tmp;
			return head;
		}
		curr = curr->next;
	}

	return head;
}
