#include "TOKENSTREAM.hpp"
#include "APP.hpp"
#include <iostream>
#include <fstream>

TokenStream::~TokenStream() { 
	for(int i = 0; i < Tokens.size(); i++) { 
		SAFE_DELETE(Tokens[i]); 
	}
	Tokens.clear(); 
}

Token TokenStream::getToken() { 
	return  * Tokens[CurIndex]; 
}

bool TokenStream::getNextToken() { 
	int size = Tokens.size(); 
	if(--size <= CurIndex) { 
		return false; 
	}
	else { 
		CurIndex++; 
		return true; 
	}
}

bool TokenStream::ungetToken(int times) { 
	for(int i = 0; i < times; i++) { 
		if(CurIndex == 0) { 
			return false; 
		}
		else { 
			CurIndex--; 
		}
	}
	return true; 
}

bool TokenStream::printTokens() { 
	std::vector<Token * >::iterator titer = Tokens.begin(); 
	while(titer != Tokens.end()) { 
		std::cout << ( * titer)->getTokenType() << ":" << std::flush; 
		if(( * titer)->getTokenType() != TOK_EOF) { 
			std::cout << ( * titer)->getTokenString() << std::endl; 
		}
		++titer; 
	}
	return true; 
}

TokenStream * LexicalAnalysis(std::string input_filename) { 
	TokenStream * tokens = new TokenStream(); 
	std::ifstream ifs; 
	std::string cur_line; 
	std::string token_str; 
	int line_num = 0; 
	bool iscomment = false; 

	ifs.open(input_filename.c_str(), std::ios::in); 
	if(!ifs) { 
		return NULL; 
	}
	while(ifs && getline(ifs, cur_line)) { 
		char next_char; 
		std::string line; 
		Token * next_token; 
		int index = 0; 
		int length = cur_line.length(); 

		while(index < length) { 
			next_char = cur_line.at(index++); 

			if(iscomment) { 
				if((length - index) < 2 || cur_line.at(index) != '*' ||
						cur_line.at(index++) != '/') continue; 
				else { 
					iscomment = false; 
				}
			}
			if(next_char == EOF) { 
				token_str = EOF; 
				next_token = new Token(token_str, TOK_EOF, line_num); 
			}
			else if(isspace(next_char)) continue; 

			else if(isalpha(next_char)) { 
				token_str += next_char; 
				next_char = cur_line.at(index++); 
				while(isalnum(next_char)) { 
					token_str += next_char; 
					next_char = cur_line.at(index++); 
					if(index == length)break; 
				}
				index--; 
				if(token_str == "int") { 
					next_token = new Token(token_str, TOK_INT, line_num); 
				}
				else if(token_str == "return") { 
					next_token = new Token(token_str, TOK_RETURN, line_num); 
				}
				else { 
					next_token = new Token(token_str, TOK_IDENTIFIER, line_num); 
				}
			}

			else if(isdigit(next_char)) { 
				if(next_char == '0') { 
					token_str += next_char; 
					next_token = new Token(token_str, TOK_DIGIT, line_num); 
				}
				else { 
					token_str += next_char; 
					next_char = cur_line.at(index++); 
					while(isdigit(next_char)) { 
						token_str += next_char; 
						next_char = cur_line.at(index++); 
					}
					next_token = new Token(token_str, TOK_DIGIT, line_num); 
					index--; 
				}
			}

			else if(next_char  == '/') { 
				token_str += next_char; 
				next_char = cur_line.at(index++); 

				if(next_char == '/') { 
					break; 
				}
				else if(next_char == '*') { 
					iscomment = true; 
					continue; 
				}
				else { 
					index--; 
					next_token = new Token(token_str, TOK_SYMBOL, line_num); 
				}
			}
			else { 
				if(next_char == '*' ||
					next_char == '+' ||
					next_char == '-' ||
					next_char == '=' ||
					next_char == ';' ||
					next_char == ',' ||
					next_char == '(' ||
					next_char == ')' ||
					next_char == '{' ||
					next_char == '}') { 
					token_str += next_char; 
					next_token = new Token(token_str, TOK_SYMBOL, line_num); 
				}
				else { 
					std::cerr << "unclear token : " << next_char << std::flush; 
					SAFE_DELETE(tokens); 
					return NULL; 
				}
			}
			tokens->pushToken(next_token); 
			token_str.clear(); 
		}
		token_str.clear(); 
		line_num++; 
	}
	
	if(ifs.eof()) { 
		tokens->pushToken(new Token(token_str, TOK_EOF, line_num)); 
	}
	ifs.close(); 
	return tokens; 
}
