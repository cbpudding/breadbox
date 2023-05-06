.PHONY: clean docs tools unix-glx

unix-glx:
	$(MAKE) -C src/system/unix-glx

docs:
	$(MAKE) -C docs

tools:
	$(MAKE) -C tools

clean:
	$(RM) -r build/