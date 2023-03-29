APPLET = testbed
SOURCES = main.c ubus.c cfg.c
OBJS =  $(patsubst %.c,%.o, $(SOURCES))

CFLAGS  += -Wall -g -Og -I./include 
LDFLAGS += 

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

.PHONY: all clean install

all: $(APPLET)

$(APPLET): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

clean:
	rm -f $(OBJS)
	rm -f ${APPLET}
	rm -f *.cmd

install:
