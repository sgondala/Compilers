all:    lex.cc parse.cc main.cc 
	./sedscript
	g++   --std=c++11 lex.cc parse.cc main.cc;
#	a.out < test > junk;

lex.cc: lex.l  Scanner.ih
	flexc++ lex.l; 
#	sed -i '/include/a #include "Parserbase.h"' Scanner.ih; 

parse.cc: grammar.y Parser.ih 
	bisonc++  grammar.y; 
#	bisonc++   --construction -V parse.y; 
#	sed -i '/insert preincludes/a #include "headers.cc"' Parserbase.h;
#	sed -i '/include "Parser.h"/a #include "definitions.cc"' Parser.ih;

.PHONY: clean
clean:
	rm -f Scanner*
	rm -f Parser*
Parser.ih: grammar.y
Parser.h:  grammar.y
Parserbase.h: grammar.y
Scanner.ih: lex.l
Scanner.h: lex.l
Scannerbase.h: lex.l
