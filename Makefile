.PHONY: clean docs tools unix-glx

unix-glx:
	$(MAKE) -C src/system/unix-glx

docs:
	$(MAKE) -C docs

clean:
	$(RM) -r build/