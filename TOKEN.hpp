#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <stdlib.h>

enum TokenType { 
	TOK_IDENTIFIER, 
	TOK_DIGIT, 
	TOK_SYMBOL, 
	TOK_INT, 
	TOK_RETURN, 
	TOK_EOF
}; 

class Token { 
	private:
		TokenType Type; 
		std::string  TokenString; 
		int Number; 
		int Line; 
	public:
		Token(std::string string, TokenType type, int line) : 
			TokenString(string), Type(type), Line(line) { 
				if(type == TOK_DIGIT) { 
					Number = atoi(string.c_str()); 
				}
				else { 
					Number = 0x7fffffff; 
				}
			}
		
		~Token() { }

		TokenType getTokenType() { return Type; }
		
		std::string getTokenString() { return TokenString; }

		int getNumberValue() { return Number; }

		int getLine() { return Line; }
}; 

#endif
