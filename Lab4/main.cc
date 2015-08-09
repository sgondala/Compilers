#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;
int lineno=1;

ofstream outFile;

int main (int argc, char** arg)
{
  
  outFile.open("code.asm");
  Parser parser;
  parser.parse();
}


