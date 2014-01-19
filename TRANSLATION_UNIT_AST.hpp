#ifndef TRANSLATION_UNIT_AST_HPP
#define TRANSLATION_UNIT_AST_HPP

#include "AST.hpp"
#include <string>
#include <vector>

class ProtoTypeAST { 
	private:
		std::string Name; 
		std::vector<std::string> Params; 

	public:
		ProtoTypeAST(const std::string& name, const std::vector<std::string>& params) :
			Name(name), Params(params) { }

		std::string getName() { return Name; }

		std::string getParamName(int i) { if(i < Params.size())return Params.at(i); return NULL; }

		int getParamNum() { return Params.size(); }
}; 

class FunctionStmtAST { 
	private:
		std::vector<VariableDeclAST * > VariableDecls; 
		std::vector<BaseAST * > StmtLists; 

	public:
		FunctionStmtAST() { }
		~FunctionStmtAST(); 

		bool addVariableDeclaration(VariableDeclAST * vdecl); 

		bool addStatement(BaseAST * stmt) { StmtLists.push_back(stmt); }

		VariableDeclAST * getVariableDecl(int i) { if(i < VariableDecls.size())return VariableDecls.at(i); else return NULL; }

		BaseAST * getStatement(int i) { if(i < StmtLists.size())return StmtLists.at(i); else return NULL; }
}; 

class FunctionAST { 
	private:
		ProtoTypeAST * Proto; 
		FunctionStmtAST * Body; 

	public:
		FunctionAST(ProtoTypeAST * proto, FunctionStmtAST * body) : Proto(proto), Body(body) { }
		~FunctionAST(); 

		std::string getName() { return Proto->getName(); }

		ProtoTypeAST * getPrototype() { return Proto; }

		FunctionStmtAST * getBody() { return Body; }
}; 

class TranslationUnitAST { 
	private:
		std::vector<ProtoTypeAST * > Prototypes; 
		std::vector<FunctionAST * > Functions; 

	public:
		TranslationUnitAST() { }
		~TranslationUnitAST(); 

		bool addPrototype(ProtoTypeAST * proto); 

		bool addFunction(FunctionAST * func); 

		bool empty(); 

		ProtoTypeAST * getPrototype(int i) { if(i < Prototypes.size())return Prototypes.at(i); else return NULL; }

		FunctionAST * getFunction(int i) { if(i < Functions.size())return Functions.at(i); else return NULL; }

}; 

#endif
