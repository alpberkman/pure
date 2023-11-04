
CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -Wno-unused-variable
DFLAGS ?= -g -ansi
SFLAGS ?= -Os -s
#FF ?= core.f pg.f
#FFEXTRA ?=  extra.f pg.f

CSRC ?= csrc
FSRC ?= fsrc
#/ff.c csrc/ff.cff_init.c csrc/ff.cff_debug.c csrc/ff.cff_interp.c

.PHONY: ff run

all: ff run

ff: $(CSRC)/*
	$(CC) $(CFLAGS) $(SFLAGS) -o build/$@ $^

run:
	clear
	cat $(FSRC)/core.f $(FSRC)/pg.f - | ./build/ff

clean:
	rm -rf build/*

