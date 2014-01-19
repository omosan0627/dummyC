#ifndef PARSER_HPP
#define PARSER_HPP

class Parser { 
	private:
		TokenStream * Tokens; 
		TranslationUnitAST * TU; 

	public:
		Paser(std::string filename); 
		~Parser() { SAFE_DELETE(TU); SAFE_DELETE(Tokens); }
		bool doParse(); 
		TranslationUnitAST& getAST(); 

	private:
		bool visitTranslationUnit(); 
		bool visitExternalDeclaration(TranslationUnitAST * tunit); 
		PrototypeAST * visitFunctionDeclearation(); 
		FunctionAST * visitFunctionDefinition(); 
		ProtoTypeAST * visitPrototype(); 
		FunctionStmtAST * visitFunctionStatement(PrototypeAST * proto); 
		VariableDeclAST * visitVariableDeclaration(); 
		BaseAST * visitStatement(); 
		BaseAST * visitExpressionStatement(); 
		BaseAST * visitJumpStatement(); 
		BaseAST * visitAssignmentExpression(); 
		BaseAST * visitAdditiveExpression(BaseAST * lhs); 
		BaseAST * visitMultiplicativeExpresion(BaseAST * lhs); 
		BaseAST * visitPostfixExpression(); 
		BaseAST * visitPrimaryExpression(); 
}; 
#endif
