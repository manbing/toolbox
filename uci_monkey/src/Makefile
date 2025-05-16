APPLET = ucimonkey
SOURCES = main.c
OBJS =  $(patsubst %.c,%.o, $(SOURCES))

CFLAGS  += -Wall -g -Og -I./include 
CFLAGS  += # -Werror
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
