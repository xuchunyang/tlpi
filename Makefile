CC	= cc
CFLAGS	= -Wall -Wextra -Wpedantic
LDFLAGS = -lm

SOURCES	    = $(wildcard *.c)
EXECUTABLES = $(patsubst %.c,%,$(SOURCES))

.PHONY: all
all: $(EXECUTABLES)
