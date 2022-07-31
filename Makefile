CC = clang
DEPS = -pthread
STD = -std=c11

ifeq (${CC}, gcc)
	WARNINGS = -Wall -Wextra
endif

ifeq (${CC}, clang)
	WARNINGS = -Weverything
endif

CFLAGS = ${STD} ${DEPS} ${WARNINGS}

main: main.o reader.o analyzer.o buffer.o printer.o
	${CC} ${CFLAGS} -o main main.o reader.o analyzer.o buffer.o printer.o

main.o: main.c reader.h buffer.h
	${CC} ${CFLAGS} -c main.c
	
reader.o: reader.c reader.h
	${CC} ${CFLAGS} -c reader.c 

analyzer.o: analyzer.c analyzer.h buffer.h
	${CC} ${CFLAGS} -c analyzer.c

buffer.o: buffer.c buffer.h
	${CC} ${CFLAGS} -c buffer.c

printer.o: printer.c printer.h
	${CC} ${CFLAGS} -c printer.c

clean:
	rm -rf *.o
	rm -rf ./main

test:
	@echo TODO: there will be some unit testing here