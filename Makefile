CC	= clang
CFLAGS	= -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lm

SOURCES	    = $(wildcard *.c)
EXECUTABLES = $(patsubst %.c,%,$(SOURCES))

.PHONY: all
all: $(EXECUTABLES)
