CC = gcc
LDFLAGS = -L/usr/lib -lssl -lcrypto
CFLAGS = -Wall

all: Igor

Igor: main.o
	${CC} -o $@ $^ ${LDFLAGS}

%.o: %.c
	${CC} -c ${CFLAGS} $^

clean:
	rm *.o
	rm Igor

.PHONY: all clean
