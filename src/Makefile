all: goc

goc: lex.yy.c parser.tab.c main.c
	gcc lex.yy.c parser.tab.c main.c -ll -o goc

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c: parser.y
	bison --verbose -d parser.y

clean: 
	rm lex.yy.c goc parser.tab.* parser.output
