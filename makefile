CC = gcc
CFLAGS = -Wall -ansi -pedantic -I Headers
SRCS = Sources/assembler.c \
       Sources/preprocessor.c \
       Sources/first_pass.c \
       Sources/analyzers.c \
       Sources/opcodes.c \
       Sources/parser.c \
       Sources/symbols.c \
       Sources/errors.c \
       Sources/globals.c \
	   Sources/second_pass.c \
	   Sources/exporter.c

OBJS = $(SRCS:.c=.o)

assembler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o assembler

Sources/assembler.o: Sources/assembler.c
	$(CC) $(CFLAGS) -c Sources/assembler.c -o Sources/assembler.o

Sources/preprocessor.o: Sources/preprocessor.c
	$(CC) $(CFLAGS) -c Sources/preprocessor.c -o Sources/preprocessor.o

Sources/first_pass.o: Sources/first_pass.c
	$(CC) $(CFLAGS) -c Sources/first_pass.c -o Sources/first_pass.o

Sources/second_pass.o: Sources/second_pass.c
	$(CC) $(CFLAGS) -c Sources/second_pass.c -o Sources/second_pass.o

Sources/exporter.o: Sources/exporter.c
	$(CC) $(CFLAGS) -c Sources/exporter.c -o Sources/exporter.o

Sources/analyzers.o: Sources/analyzers.c
	$(CC) $(CFLAGS) -c Sources/analyzers.c -o Sources/analyzers.o

Sources/opcodes.o: Sources/opcodes.c
	$(CC) $(CFLAGS) -c Sources/opcodes.c -o Sources/opcodes.o

Sources/parser.o: Sources/parser.c
	$(CC) $(CFLAGS) -c Sources/parser.c -o Sources/parser.o

Sources/symbols.o: Sources/symbols.c
	$(CC) $(CFLAGS) -c Sources/symbols.c -o Sources/symbols.o

Sources/errors.o: Sources/errors.c
	$(CC) $(CFLAGS) -c Sources/errors.c -o Sources/errors.o

Sources/globals.o: Sources/globals.c
	$(CC) $(CFLAGS) -c Sources/globals.c -o Sources/globals.o

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.am" -type f -delete
	find . -name "*.ext" -type f -delete
	find . -name "*.ent" -type f -delete
	find . -name "*.ob" -type f -delete