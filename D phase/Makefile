all: 
	bison --yacc --defines -v --output=parser.c parser.y

	flex --outfile=scanner.c scanner.I	

	gcc -o run scanner.c parser.c SymbolTable.h quadlibrary.h Tcode.h

clean:
	rm parser.c parser.h run scanner.c parser.output BinaryCode.lol
