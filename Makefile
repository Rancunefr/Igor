CC = gcc
LDFLAGS = -lssl -lcrypto
CFLAGS = -Wall

all: Igor

Igor: main.o connexion.o irc.o pretty_print.o parse_msg.o bot.o
	${CC} -o $@ $^ ${LDFLAGS}

%.o: %.c
	${CC} -c ${CFLAGS} $^

clean:
	rm -f *.o
	rm -f Igor

.PHONY: all clean
