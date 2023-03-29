PROG = waninfo
SOURCES = main.c
OBJS =  $(patsubst %.c,%.o, $(SOURCES))

CFLAGS  += -Wall -g -Og -I./include 
LDFLAGS += 

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

.PHONY: all clean install

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

clean:
	rm -f $(OBJS)
	rm -f ${PROG}
	rm -f *.cmd

install:
