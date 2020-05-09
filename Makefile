CC	= cc
CFLAGS	= -Wall -Wextra -Wpedantic -pthread
LDFLAGS = -lm -ldl

SOURCES	    = $(wildcard *.c)
EXECUTABLES = $(patsubst %.c,%,$(SOURCES))

.PHONY: all
all: $(EXECUTABLES)

.PHONY: clean
clean:
	rm -f $(EXECUTABLES)
