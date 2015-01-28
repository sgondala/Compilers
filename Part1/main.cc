#include <iostream>
#include "Scanner.h"
using namespace std;
int main()
{
  Scanner scanner;   // define a Scanner object
  while (int token = scanner.lex()) // get all tokens
    {
      string const &text = scanner.matched();
      switch (token)
	{
	case Scanner::IDENTIFIER:
	  cout << "identifier: " << text << '\n';
	  break;
	case Scanner::TypeSpecifier:
	  cout << "TypeSpecifier: " << text << '\n';
	  break;
	case Scanner::Delimiter:
	  cout << "Delimiter: " << text << '\n';
	  break;
	case Scanner::Parenthesis:
	  cout << "Parenthesis: " << text << '\n';
	  break;
	case Scanner::Comma:
	  cout << "Comma: " << text << '\n';
	  break;
	case Scanner::SquareBraces:
	  cout << "SquareBraces: " << text << '\n';
	  break;
	case Scanner::FlowerBraces:
	  cout << "FlowerBraces: " << text << '\n';
	  break;
	case Scanner::Integer:
	  cout << "Integer: " << text << '\n';
	  break;
	case Scanner::Float:
	  cout << "Float: " << text << '\n';
	  break;
        case Scanner::String:
	  cout << "String: " << text << '\n';
	  break;
        case Scanner::Return:
	  cout << "Return: " << text << '\n';
	  break;
        case Scanner::BooleanOP:
	  cout << "BooleanOP: " << text << '\n';
	  break;
        case Scanner::IncrementOP:
	  cout << "IncrementOP: " << text << '\n';
	  break;
        case Scanner::Assignment:
	  cout << "Assignment: " << text << '\n';
	  break;
        case Scanner::MathematicalOperators:
	  cout << "MathematicalOperators: " << text << '\n';
	  break;
	case Scanner::UnaryOperators:
	  cout << "UnaryOperators: " << text << '\n';
	  break;
        case Scanner::Condition:
	  cout << "Condition: " << text << '\n';
	  break;    
        case Scanner::Loop:
	  cout << "loop: "  <<  text << '\n';
	  break; 
        case Scanner::Comment:
	  cout << "Comment: "  <<  text << '\n';
	  break;
	default:
	  cout << "char. token: " << text << '\n';
	}
    }
}


