#include <stdio.h>
#include <stdlib.h>

#include "linkedlist_handler.h"

struct ll_thread_obj* head;

/* Simple test for the linked list handler */
int main(void) {
	struct ll_thread_obj* newitems = malloc(sizeof(struct ll_thread_obj) * 5);

	newitems[0].filepath = "This";
	newitems[1].filepath = "is";
	newitems[2].filepath = "a";
	newitems[3].filepath = "test";
	newitems[4].filepath = "_";

	ll_add(&head, &newitems[0]);
	ll_add(&head, &newitems[1]);
	ll_add(&head, &newitems[2]);
	ll_add(&head, &newitems[3]);
	ll_add(&head, &newitems[4]);

	struct ll_thread_obj* curr = head;
	while (curr != NULL) {
		printf("%s ", curr->filepath);
		curr = curr->next;
	}
	printf("\n");

	/* This should cause a memory leak, do not do it in actual use */
	printf("%s\n", ll_find_and_remove(&head, "This")->filepath);
	printf("%s\n", ll_find_and_remove(&head, "a")->filepath);
	printf("%s\n", ll_find_and_remove(&head, "_")->filepath);

	curr = head;
	while (curr != NULL) {
		printf("%s ", curr->filepath);
		curr = curr->next;
	}
	printf("\n");

	return 0;
}
