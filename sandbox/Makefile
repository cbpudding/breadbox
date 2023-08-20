CFLAGS=-I../include

ifneq (,$(findstring win32,$(MAKECMDGOALS)))
CFLAGS+= -lm -lopengl32
else
CFLAGS+= -lGL -lm -lX11
endif

.PHONY: unix-glx

unix-glx: main.c ../build/unix-glx/libbreadbox.a
	$(CC) $(CFLAGS) -o ../build/unix-glx/sandbox $^

win32-wgl: main.c ../build/win32-wgl/libbreadbox.a
	$(CC) $(CFLAGS) -o ../build/win32-wgl/sandbox.exe $^

../build/unix-glx/libbreadbox.a:
	$(MAKE) -C .. unix-glx

../build/win32-wgl/libbreadbox.a:
	$(MAKE) -C .. win32-wgl