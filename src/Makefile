all: goc

goc: lex.yy.c parser.tab.c main.c tree.c weed.c pretty.c symbol.c type.c code.c codeStructs.c codeTypes.c codeDeclarations.c codeStatements.c codeIdentifiers.c codeExpressions.c
	gcc lex.yy.c parser.tab.c main.c tree.c weed.c pretty.c symbol.c type.c code.c codeStructs.c codeTypes.c codeDeclarations.c codeStatements.c codeIdentifiers.c codeExpressions.c -o goc

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c: parser.y
	bison --verbose -d parser.y

clean: 
	rm lex.yy.c goc parser.tab.* parser.output
