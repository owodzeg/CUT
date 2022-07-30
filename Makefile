CC = clang
STD = -std=c11
DEPS = -pthread
WARNINGS = -Weverything
CFLAGS = ${STD} ${DEPS} ${WARNINGS}

main: main.o reader.o
	${CC} ${CFLAGS} -o main main.o reader.o

main.o: main.c reader.h
	${CC} ${CFLAGS} -c main.c
	
reader.o: reader.c reader.h
	${CC} ${CFLAGS} -c reader.c 
	
clean:
	rm -rf *.o
	rm -rf ./main

test:
	@echo TODO: there will be some unit testing here
