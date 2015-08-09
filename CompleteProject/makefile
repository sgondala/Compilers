all:  clean lex.cc parse.cc main.cc
	./sedscript
	g++ --std=c++11 -o out lex.cc parse.cc main.cc;
#	a.out < test > junk;

run: machine.cc code.asm out
	g++ -g3 -O0 machine.cc

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
	rm -f *out
	rm -f code.asm
Parser.ih: grammar.y
Parser.h:  grammar.y
Parserbase.h: grammar.y
Scanner.ih: lex.l
Scanner.h: lex.l
Scannerbase.h: lex.l
