#ifndef TOUCHBOOST_UD
#define TOUCHBOOST_UD

int ud_find_devices(void (*callback)(const char*));
int ud_monitor_devices(void (*callback)(const char*, const char*));

#endif
