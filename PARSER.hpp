#ifndef PARSER_HPP
#define PARSER_HPP

#include "TRANSLATION_UNIT_AST.hpp"
#include "TOKENSTREAM.hpp"

class Parser { 
	public:
		TokenStream * Tokens; 
		TranslationUnitAST * TU; 

	public:
		Parser(std::string filename); 
		~Parser() { SAFE_DELETE(TU); SAFE_DELETE(Tokens); }
		bool doParse(); 
		TranslationUnitAST& getAST(); 

	private:
		bool visitTranslationUnit(); 
		bool visitExternalDeclaration(TranslationUnitAST * tunit); 
		ProtoTypeAST * visitFunctionDeclearation(); 
		FunctionAST * visitFunctionDefinition(); 
		ProtoTypeAST * visitPrototype(); 
		FunctionStmtAST * visitFunctionStatement(ProtoTypeAST * proto); 
		VariableDeclAST * visitVariableDeclaration(); 
		BaseAST * visitStatement(); 
		BaseAST * visitExpressionStatement(); 
		BaseAST * visitJumpStatement(); 
		BaseAST * visitAssignmentExpression(); 
		BaseAST * visitAdditiveExpression(BaseAST * lhs); 
		BaseAST * visitMultiplicativeExpression(BaseAST * lhs); 
		BaseAST * visitPostfixExpression(); 
		BaseAST * visitPrimaryExpression(); 
}; 

#endif
