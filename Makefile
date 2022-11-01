CC = gcc
LDFLAGS = -lssl -lcrypto
CFLAGS = -Wall

all: Igor

Igor: main.o connexion.o
	${CC} -o $@ $^ ${LDFLAGS}

%.o: %.c
	${CC} -c ${CFLAGS} $^

clean:
	rm *.o
	rm Igor

.PHONY: all clean
