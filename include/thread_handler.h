#ifndef TOUCHBOOST_TD
#define TOUCHBOOST_TD

#include "linkedlist_handler.h"

void td_state_set(int state);
struct ll_thread_obj* td_thread_create(const char* filepath);
void td_thread_delete(struct ll_thread_obj* thread_obj);
void td_init(void);
void td_freq_ctls_init(int argc, const char** argv);

#endif
