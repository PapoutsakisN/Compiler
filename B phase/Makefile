all:
	bison --yacc --defines -v --output=parser.c parser.y

	flex --outfile=scanner.c scanner.I	
			
	gcc -o run scanner.c parser.c

clean:
	rm parser.c parser.h run scanner.c parser.output
