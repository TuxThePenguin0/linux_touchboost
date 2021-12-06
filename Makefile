CFLAGS=-Wall -Werror -O2

INCLUDES=-Iinclude
LIBS=-lrt -lpthread -ludev
OBJS := build/linkedlist_handler.o build/main.o build/thread_handler.o build/udev_handler.o

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

.PHONY: install install-openrc install-systemd uninstall clean

build/touchboost: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

build/%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS) $(INCLUDES)

install: build/touchboost
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 build/touchboost $(DESTDIR)$(PREFIX)/bin/touchboost
	install -m 755 scripts/touchboost_detect $(DESTDIR)$(PREFIX)/bin/touchboost_detect

install-openrc: install
	install -d $(DESTDIR)/etc/init.d
	install -d $(DESTDIR)/etc/conf.d
	sed 's-PREFIX-$(PREFIX)-g' scripts/touchboost_openrc > $(DESTDIR)/etc/init.d/touchboost
	cp scripts/touchboost_openrc_conf $(DESTDIR)/etc/conf.d/touchboost
	chmod 755 $(DESTDIR)/etc/init.d/touchboost
	chmod 644 $(DESTDIR)/etc/conf.d/touchboost

install-systemd: install
	install -d $(DESTDIR)/usr/lib/systemd/system
	sed 's-PREFIX-$(PREFIX)-g' scripts/touchboost_systemd > $(DESTDIR)/usr/lib/systemd/system/touchboost.service
	chmod 644 $(DESTDIR)/usr/lib/systemd/system/touchboost.service

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/touchboost
	rm $(DESTDIR)$(PREFIX)/bin/touchboost_detect
	rm -f $(DESTDIR)/etc/init.d/touchboost
	rm -f $(DESTDIR)/etc/conf.d/touchboost
	rm -f $(DESTDIR)/usr/lib/systemd/system/touchboost.service

clean:
	rm -f build/*
