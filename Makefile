.PHONY: clean tools

tools: build/tools/bbconv build/tools/bbpack

build/tools:
	mkdir -p build/tools

build/tools/%: tools/%.c build/tools
	$(CC) -o $@ $<

clean:
	$(RM) -r build/