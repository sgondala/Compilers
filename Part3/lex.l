%%
[ \t\n]+                                    // skip white space chars. 
VOID|void								        return Parser::VOID;
INT|int 										return Parser::INT;
FLOAT|float 									return Parser::FLOAT;
//STRING|string 								return Parser::INT;
RETURN|return									return Parser::RETURN;
(&{1,2})										return Parser::AND_OP;
(\|{1,2})										return Parser::OR_OP;
"==" 											return Parser::EQ_OP;
"!=" 											return Parser::NE_OP;
"<=" 											return Parser::LE_OP;
">=" 											return Parser::GE_OP;
"<"|">"											return matched()[0];
"++"|"--"										return Parser::INC_OP;
IF|if 											return Parser::IF;
ELSE|else 										return Parser::ELSE;
WHILE|while										return Parser::WHILE;
FOR|for											return Parser::FOR;
"("|")"                                        	return matched()[0];
[\[\]]                                         	return matched()[0];
[{}]                                         	return matched()[0];
","                                           	return matched()[0];
";"                                           	return matched()[0];           
"="												return matched()[0];
[/*+]											return matched()[0];
[-!]											return matched()[0];
([1-9][0-9]*)|(0[bB][01]+)|(0[0-7]+)|(0[xX][0-9a-fA-F]+)|0 {Parser::d_val__ = std::string(matched()); return Parser::INT_CONSTANT;}
([0-9]*\.[0-9]+([eE][\+-]?[0-9]+)?)|([0-9]+\.)|([0-9]+\.[eE][\+-]?[0-9]+) {Parser::d_val__ = std::string(matched());return Parser::FLOAT_CONSTANT;}
\"([^\n\"]*\\\")*[^\n\"]*\" {Parser::d_val__ = std::string(matched());return Parser::STRING_LITERAL;}
[[:alpha:]_][[:alnum:]_]* {Parser::d_val__ = std::string(matched());return Parser::IDENTIFIER;}
. {}
