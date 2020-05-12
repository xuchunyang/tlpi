CC	= cc
CFLAGS	= -Wall -Wextra -Wpedantic -pthread
LDLIBS  = -lm -ldl -lncurses

SOURCES	    = $(wildcard *.c)
EXECUTABLES = $(patsubst %.c,%,$(SOURCES))

.PHONY: all
all: $(EXECUTABLES)

.PHONY: clean
clean:
	rm -f $(EXECUTABLES)
