assembler: assembler.o errors.o preprocessor.o
	gcc -Wall -ansi -pedantic assembler.o errors.o preprocessor.o -o assembler

assembler.o: assembler.c errors.h preprocessor.h
	gcc -Wall -ansi -pedantic -c assembler.c -o assembler.o

errors.o: errors.c errors.h
	gcc -Wall -ansi -pedantic -c errors.c -o errors.o

preprocessor.o: preprocessor.c preprocessor.h
	gcc -Wall -ansi -pedantic -c preprocessor.c -o preprocessor.o