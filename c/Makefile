
PROGRAMA=threads
PROGRAMA32=threads

LIBS= -lpthread

CC=gcc

SOURCE= threads.c

all32: clean compile32


all: clean compile

clean:
	rm -f *.o
	rm -f $(PROGRAMA)

compile32:
	$(CC) -m32  $(SOURCE) $(LIBS) -o $(PROGRAMA32)

compile:
	$(CC) $(SOURCE) $(LIBS) -o $(PROGRAMA)

run:
	$(PROGRAMA)

