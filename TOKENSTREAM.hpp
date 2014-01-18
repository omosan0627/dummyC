#ifndef TOKENSTREAM_HPP
#define TOKENSTREAM_HPP

#include "Token.hpp"
#include <vector>
#include <string>

class TokenStream { 
	private:
		std::vector<Token * > Tokens; 
		int CurIndex; 

	public:
		TokenStream() : CurIndex(0) { }
		~TokenStream(); 

		bool ungetToken(int Times = 1); 
		bool getNextToken(); 
		bool pushToken(Token * token) { 
			Tokens.push_back(token); 
			return true; 
		}
		Token getToken(); 

		TokenType getCurType() { return Tokens[CurIndex]->getTokenType(); }

		std::string getCurString() { return Tokens[CurIndex]->getTokenString(); }

		int getCurNumVal() { return Tokens[CurIndex]->getNumberValue(); }

		int getCurIndex() { return CurIndex; }

		bool applyTokenIndex(int index) { CurIndex = index; return true; }
		bool printTokens(); 

}; 

TokenStream * LexicalAnalysis(std::string input_filename); 

#endif
