CFLAGS=-std=c99 -I../../../include -pedantic -Wall -Wextra

BACKEND=unix-glx

SRCDIR=../..
OUTDIR=../../../build/$(BACKEND)

$(OUTDIR)/libbreadbox.a: $(OUTDIR)/list.o $(OUTDIR)/log.o $(OUTDIR)/model.o $(OUTDIR)/subscriptions.o $(OUTDIR)/utility.o $(OUTDIR)/system/input.o $(OUTDIR)/system/interface.o $(OUTDIR)/system/view.o
	$(AR) rcs $@ $^

$(OUTDIR)/system/%.o: $(SRCDIR)/system/$(BACKEND)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR)/%.o: $(SRCDIR)/%.c $(OUTDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR):
	mkdir -p $(OUTDIR)/system