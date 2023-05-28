CFLAGS=-I../../../include

BACKEND=unix-glx

SRCDIR=../..
OUTDIR=../../../build/$(BACKEND)

$(OUTDIR)/libbreadbox.a: $(OUTDIR)/model.o $(OUTDIR)/subscriptions.o $(OUTDIR)/system/interface.o $(OUTDIR)/system/resource.o $(OUTDIR)/system/view.o
	$(AR) rcs $@ $^

$(OUTDIR)/system/%.o: $(SRCDIR)/system/$(BACKEND)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR)/%.o: $(SRCDIR)/%.c $(OUTDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR):
	mkdir -p $(OUTDIR)/system