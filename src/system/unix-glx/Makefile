CFLAGS=-I../../../include

SRCDIR=../..
OUTDIR=../../../build/unix-glx

$(OUTDIR)/libbreadbox.a: $(OUTDIR)/breadbox.o $(OUTDIR)/model.o $(OUTDIR)/subscriptions.o $(OUTDIR)/system/interface.o $(OUTDIR)/system/view.o
	$(AR) rcs $@ $^

$(OUTDIR)/system/%.o: $(SRCDIR)/system/unix-glx/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR)/%.o: $(SRCDIR)/%.c $(OUTDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR):
	mkdir -p $(OUTDIR)/system