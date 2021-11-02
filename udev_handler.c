#include <stddef.h>
#include <fcntl.h>
#include <libudev.h>

#include "udev_handler.h"

/* TODO: Add error handling */

/* Get a list of currently connected devices and execute callback for each one */
int ud_find_devices(void (*callback)(const char*)) {
	struct udev* ud_context = udev_new();
	struct udev_enumerate* ud_enum = udev_enumerate_new(ud_context);
	udev_enumerate_add_match_subsystem(ud_enum, "input");
	udev_enumerate_scan_devices(ud_enum);

	struct udev_list_entry* ud_enum_found = udev_enumerate_get_list_entry(ud_enum);
	struct udev_device* ud_enum_dev;
	const char* ud_enum_devnode;
	while (ud_enum_found != NULL) {
		ud_enum_dev = udev_device_new_from_syspath(ud_context, udev_list_entry_get_name(ud_enum_found));
		ud_enum_devnode = udev_device_get_devnode(ud_enum_dev);
		if (ud_enum_devnode != NULL) {
			(*callback)(ud_enum_devnode);
		}
		udev_device_unref(ud_enum_dev);
		ud_enum_found = udev_list_entry_get_next(ud_enum_found);
	}

	udev_enumerate_unref(ud_enum);
	udev_unref(ud_context);

	return 0;
}

/* Watch for devices being added or removed and execute callback for each one */
int ud_monitor_devices(void (*callback)(const char*, const char*)) {
	struct udev* ud_context = udev_new();
	struct udev_monitor* ud_monitor = udev_monitor_new_from_netlink(ud_context, "udev");
	int ud_monitor_fd = udev_monitor_get_fd(ud_monitor);

	fcntl(ud_monitor_fd, F_SETFL, fcntl(ud_monitor_fd, F_GETFL) & ~O_NONBLOCK);
	udev_monitor_filter_add_match_subsystem_devtype(ud_monitor, "input", NULL);
	udev_monitor_enable_receiving(ud_monitor);

	struct udev_device* ud_monitor_dev;
	const char* ud_monitor_devnode;

	while (1) {
		ud_monitor_dev = udev_monitor_receive_device(ud_monitor);
		ud_monitor_devnode = udev_device_get_devnode(ud_monitor_dev);

		if (ud_monitor_devnode != NULL) {
			(*callback)(udev_device_get_action(ud_monitor_dev), ud_monitor_devnode);
		}

		udev_device_unref(ud_monitor_dev);
	}

	udev_monitor_unref(ud_monitor);
	udev_unref(ud_context);

	return 0;
}
