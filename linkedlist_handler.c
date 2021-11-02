#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "linkedlist_handler.h"
#include "thread_handler.h"

/* ll_add - Add item to the end of a list, returns newly added item */
struct ll_thread_obj* ll_add(struct ll_thread_obj** head, struct ll_thread_obj* newitem) {
	/* Don't want to be accessing unintialized data */
	newitem->next = NULL;

	/* If the list is empty then put newitem at the start */
	if (*head == NULL) {
		*head = newitem;
		return newitem;
	}

	/* Find the end of the list and add newitem there */
	struct ll_thread_obj* curr = *head;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = newitem;

	return newitem;
}

/* ll_find_and_remove - Find the first matching item in a list and remove it, returns removed item */
struct ll_thread_obj* ll_find_and_remove(struct ll_thread_obj** head, const char* filepath) {
	/* If the list is empty we have nothing to remove */
	if (*head == NULL) {
		return NULL;
	}

	/* If the head is the item we want to remove then move the head to the next value and return the old head */
	if (strcmp((*head)->filepath, filepath) == 0) {
		struct ll_thread_obj* tmp = *head;
		*head = (*head)->next;
		return tmp;
	}

	/* Find the first matching item, cut it out of the list and return it */
	struct ll_thread_obj* curr = *head;
	while (curr->next != NULL) {
		if (strcmp(curr->next->filepath, filepath) == 0) {
			struct ll_thread_obj* tmp = curr->next;
			curr->next = curr->next->next;
			return tmp;
		}
		curr = curr->next;
	}

	return NULL;
}
