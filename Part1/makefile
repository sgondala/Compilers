all:    lex.cc  main.cc 
	g++   --std=c++0x lex.cc main.cc

lex.cc: ass1.l 
	./cond_remove_scannerh
	flexc++ ass1.l; 
	sed -i '/int lex()/r enums' Scanner.h
