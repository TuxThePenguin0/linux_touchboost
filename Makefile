CFLAGS=-Wall -Werror

INCLUDES=-Iinclude
LIBS=-lrt -lpthread -ludev
OBJS := build/linkedlist_handler.o build/main.o build/thread_handler.o build/udev_handler.o

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

.PHONY: install uninstall clean

build/touchboost: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

build/%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS) $(INCLUDES)

install: build/touchboost
	install -d $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)/etc/init.d
	install -m 755 build/touchboost $(DESTDIR)$(PREFIX)/bin/touchboost
	install -m 755 scripts/touchboost_detect $(DESTDIR)$(PREFIX)/bin/touchboost_detect
	sed 's-PREFIX-$(PREFIX)-g' scripts/touchboost_openrc > $(DESTDIR)/etc/init.d/touchboost
	chmod 755 $(DESTDIR)/etc/init.d/touchboost

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/touchboost
	rm $(DESTDIR)$(PREFIX)/bin/touchboost_detect
	rm $(DESTDIR)/etc/init.d/touchboost

clean:
	rm -f build/*
