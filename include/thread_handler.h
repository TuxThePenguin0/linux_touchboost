#ifndef TOUCHBOOST_TD
#define TOUCHBOOST_TD

#include "linkedlist_handler.h"

void td_thread_create(struct ll_thread_obj* thread_obj);
void td_thread_delete(struct ll_thread_obj* thread_obj);
void td_init();
void td_freq_ctls_init(int argc, const char** argv);

#endif
