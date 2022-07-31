CC = clang
STD = -std=c11
DEPS = -pthread
WARNINGS = -Weverything
CFLAGS = ${STD} ${DEPS} ${WARNINGS}

main: main.o reader.o analyzer.o buffer.o
	${CC} ${CFLAGS} -o main main.o reader.o analyzer.o buffer.o

main.o: main.c reader.h buffer.h
	${CC} ${CFLAGS} -c main.c
	
reader.o: reader.c reader.h
	${CC} ${CFLAGS} -c reader.c 

analyzer.o: analyzer.c analyzer.h buffer.h
	${CC} ${CFLAGS} -c analyzer.c

buffer.o: buffer.c buffer.h
	${CC} ${CFLAGS} -c buffer.c

clean:
	rm -rf *.o
	rm -rf ./main

test:
	@echo TODO: there will be some unit testing here