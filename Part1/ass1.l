
// There may be some flexer directives here

%%
[ \t\n]+                                    // skip white space chars. 
VOID|void|INT|int|FLOAT|float|STRING|string     return TypeSpecifier;
RETURN|return					return Return;
(&{1,2})|(\|{1,2})|"=="|"!="|"<="|">="|"<"|">"	return BooleanOP;
"++"|"--"					return IncrementOP;
IF|if|ELSE|else 				return Condition;
WHILE|while|FOR|for				return Loop;
[()]                                         	return Parenthesis;
[\[\]]                                         	return SquareBraces;
[{}]                                         	return FlowerBraces;
,                                           	return Comma;
;                                           	return Delimiter;           
=						return Assignment;
[/*+]						return MathematicalOperators;
[-!]						return UnaryOperators;
\/\*[^\*]*(\*([^\/\*][^\*]*)?)*\*\/		return Comment;
\"(\\.|[^\\"])*\"				return String;
[+-]?([1-9][0-9]*)|(0[Bb][01]*)|(0[0-7]*)|(0[xX][0-9a-fA-F]*) return Integer;
[+-]?([0-9]+\.[0-9]*[eE]?[+-]?[0-9]*)|(\.[0-9]+[eE]?[+-]?[0-9]*)|(\.[eE][+-]?[0-9]*) return Float;
[[:alpha:]_][[:alpha:][:digit:]_]*          return IDENTIFIER;
.                                           return matched()[0];


// IDENTIFIERs have been defined as any character from the alphabet or
// an underscore followed by 0 or more occurrences of any alphanumeric
// character or an underscore. :alpha: and :digit: are predefined.

// NUMBER and  IDENTIFIER are values  of an enum  type and have  to be
// defined. We shall show elsewhere how.

// matched () returns a string containing the detected lexeme - the 
// equivalent of yytext in lex.
