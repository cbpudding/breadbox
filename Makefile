.PHONY: clean docs sandbox-unix-glx tools unix-glx

sandbox-unix-glx:
	$(MAKE) -C sandbox unix-glx

unix-glx:
	$(MAKE) -C src/system/unix-glx

docs:
	$(MAKE) -C docs

tools:
	$(MAKE) -C tools

clean:
	$(RM) -r build/