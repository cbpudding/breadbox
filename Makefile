.PHONY: clean tools unix-glx

unix-glx:
	$(MAKE) -C src/system/unix-glx

tools: build/tools/bbconv build/tools/bbpack

build/tools:
	mkdir -p build/tools

build/tools/%: tools/%.c build/tools
	$(CC) -o $@ $<

clean:
	$(RM) -r build/