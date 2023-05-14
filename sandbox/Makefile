CFLAGS=-I../include -lGL -lm -lX11

.PHONY: unix-glx

unix-glx: main.c ../build/unix-glx/libbreadbox.a
	$(CC) $(CFLAGS) -o ../build/unix-glx/sandbox $^

../build/unix-glx/libbreadbox.a:
	$(MAKE) -C .. unix-glx