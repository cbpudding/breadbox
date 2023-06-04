CFLAGS=-I../include

.PHONY: all

all: ../build/tools/bbconv ../build/tools/bbpack

../build/tools:
	mkdir -p $@

../build/tools/%: %.c ../build/tools
	$(CC) $(CFLAGS) -o $@ $<
