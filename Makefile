CC = gcc
LDFLAGS = -lssl -lcrypto
CFLAGS = -Wall

all: Igor

Igor: main.o connexion.o irc.o pretty_print.o parse_msg.o
	${CC} -o $@ $^ ${LDFLAGS}

%.o: %.c
	${CC} -c ${CFLAGS} $^

clean:
	rm *.o
	rm Igor

.PHONY: all clean
