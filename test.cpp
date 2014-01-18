#include <iostream>
#include "TOKENSTREAM.hpp"

using namespace std; 

int main() { 
	TokenStream * code = LexicalAnalysis("test.dummyc"); 
	code->printTokens(); 
}

