CC = gcc
LDFLAGS = -lssl -lcrypto -liniparser4
CFLAGS = -g -Wall -I/usr/include/iniparser4

all: Igor

Igor: main.o connexion.o irc.o pretty_print.o parse_msg.o bot.o
	${CC} -o $@ $^ ${LDFLAGS}

%.o: %.c
	${CC} -c ${CFLAGS} $^

clean:
	rm -f *.o
	rm -f Igor

.PHONY: all clean
