.PHONY: clean docs tools unix-glx win32-wgl

unix-glx:
	$(MAKE) -C src/system/unix-glx

win32-wgl:
	$(MAKE) -C src/system/win32-wgl

docs:
	$(MAKE) -C docs

clean:
	$(RM) -r build/