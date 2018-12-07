##@author by Sebastian WIEDEMANN 1425647
CC = gcc
DEFS = ##-D_XOPEN_SOURCE=500 -D_BSD_SOURCE
CFLAGS = -g -std=c99# -Wall -pedantic $(DEFS)
LDFLAGS = -g
OBJECTFILES = *.o
EXECS = main 
.PHONY: all clean

all: main

main: $(OBJECTFILES)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c  $<

clean:
	rm -f $(OBJECTFILES) $(EXECS)
