HEADERS	+=	kernel.h\
			io.h

VGADIR	:= $(KERNELDIR)/vga
IODIR	:= $(KERNELDIR)/io

VPATH	+= $(VGADIR) $(IODIR)

include $(VGADIR)/files.mk
include $(IODIR)/files.mk