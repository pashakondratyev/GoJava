all: goc

goc: lex.yy.c mini.tab.c main.c
	gcc lex.yy.c mini.tab.c main.c -ll -o goc

lex.yy.c: scanner.l
	flex scanner.l

mini.tab.c: parser.y
	bison --verbose -d parser.y

clean: 
	rm lex.yy.c goc mini.tab.* mini.output
