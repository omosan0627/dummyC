#include <iostream>
#include "TOKENSTREAM.hpp"
#include "PARSER.hpp"

using namespace std; 

int main() { 
	Parser parser("test.dummyc");
	parser.Tokens->printTokens(); 
	if(!parser.doParse())cout << "FAIL" << endl; 
	else cout << "SUCCESS" << endl; 
	TranslationUnitAST result = parser.getAST(); 
	if(result.empty())cout << "empty" << endl; 
	else { 
		for(int i = 0; i < 2; i++) { 
			cout << result.getFunction(i)->getName() << endl;
		}
	}
}

