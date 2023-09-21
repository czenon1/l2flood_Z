CC ?= cc
INSTALL ?= install
PREFIX ?= /usr/local

wrapper = Zl2flood_alt
project = l2flood

LDFLAGS = -lbluetooth

parallel:
	$(CC) $(project).c -fopenmp $(LDFLAGS) -o $(project)
	$(CC) $(wrapper).c -fopenmp $(LDFLAGS) -o $(wrapper)
serial:
	$(CC) $(project).c $(LDFLAGS) -o $(project)
	$(CC) $(wrapper).c $(LDFLAGS) -o $(wrapper)
clean:
	rm -f ./$(project)
	rm -f ./$(wrapper)
install:
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	$(INSTALL) ./$(project) "$(DESTDIR)$(PREFIX)/bin/$(project)"
	$(INSTALL) ./$(wrapper) "$(DESTDIR)$(PREFIX)/bin/$(project)"
.PHONY: parallel serial clean install
