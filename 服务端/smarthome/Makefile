TOPDIR	:= $(shell pwd)
INCDIR	:= -I$(TOPDIR)/include/jpegInclude -I$(TOPDIR)/include
# need to be modified according to your rootfs path
DSTDIR	:= /home/farsight/Documents/board/nfs/rootfs/

# need to be modified according to your cross compiler
CROSS_COMPILE ?= arm-linux-#arm-none-gnueabi- 
CC := $(CROSS_COMPILE)gcc
#CC := gcc
CFLAGS += -Wall $(INCDIR)
LFLAGS += $(LIBDIR)
#LFLAGS += -lpthread -ljpeg -L/home/kiki/jpeg/lib
#LFLAGS += -lpthread -ljpeg -L./ -lCamera
LFLAGS += -lpthread -ljpeg -L./ -lCamera -L$(DSTDIR)lib/
export CC CFLAGS

SRCS	:= $(wildcard *.c)
OBJS	:= $(patsubst %.c, %.o, $(SRCS))
# need to be modified according to your subdirs
DEPS	:= $(patsubst %.o, %.d, $(OBJS))

# need to be modified according to your target
APPS	:= QtVideoSrv2

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(APPS)
	cp $(APPS) $(DSTDIR)

clean:
	- $(RM) $(DEPS) $(OBJS) $(APPS) *.d.*

.PHONY: clean

sinclude $(SRCS:.c=.d)

%.d:%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,/($*/).o[ :]*,/1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
