
CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -Wno-unused-variable
DFLAGS ?= -g -ansi
SFLAGS ?= -Os -s
FF ?= core.f libs/term.f ff2.f pg.f
FFEXTRA ?=  extra.f pg.f

.PHONY: ff run

all: ff run

ff: main.c ff.c ff_init.c ff_debug.c ff_interp.c
	$(CC) $(CFLAGS) $(SFLAGS) -o $@ $^

run:
	clear
	cat $(FF) - | ./ff

clean:
	rm -rf *.o *.optimized *.out debug ff *.orig

