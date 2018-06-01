
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/man

CFLAGS = -std=gnu99 -pedantic -Wall
CC = cc


agenda: agenda.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f agenda

install: agenda
	install agenda $(DESTDIR)$(PREFIX)/bin/agenda
	install -m 644 agenda.1 $(DESTDIR)$(MANPREFIX)/man1/agenda.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/agenda
	rm -f $(DESTDIR)$(MANPREFIX)/man1/agenda.1

.PHONY: clean install uninstall
