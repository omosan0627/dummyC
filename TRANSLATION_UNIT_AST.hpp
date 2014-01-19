#ifndef TRANSLATION_UNIT_AST_HPP
#define TRANSLATION_UNIT_AST_HPP

class ProtoTypeAST { 
	private:
		std::string Name; 
		std::vector<std::string> Params; 

	public:
		PrototypeAST(const std::string& name, const std::vector<std::string>& params) :
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

		bool addStatement(BaseAST * stmt) { stmtLists.push_back(stmt); }

		VariableDeclAST * getVariableDecl(int i) { if(i < VariableDecls.size())return VariableDecls.at(i); else return NULL; }

		BaseAST * getStatement(int i) { if(i < StmtLists.size())return StmtLists.at(i); else return NULL; }
}; 

class FunctionAST { 
	private:
		PrototypeAST * Proto; 
		FunctionStmtAST * Body; 

	public:
		FunctionAST(PrototypeAST * proto, FunctionStmtAST * body) : Proto(proto), Body(body) { }
		~FunctionAST(); 

		std::string getName() { return Proto->getName(); }

		PrototypeAST * getPrototype() { return Proto; }

		FunctionStmtAST * getBody() { return Body; }
}; 

class TranslationUnitAST { 
	private:
		std::vector<PrototypeAST * > Prototypes; 
		std::vector<FunctionAST * > Functions; 

	public:
		TranslationUnitAST() { }
		~TranslationUnitAST(); 

		bool addPrototype(PrototypeAST * proto); 

		bool addFunction(FunctionAST * func); 

		bool empty(); 

		PrototypeAST * getPrototype(int i) { if(i < Prototypes.size())return Prototypes.at(i); else return NULL; }

		FunctionAST * getFunction(int i) { if(i < Functions.size())return Functions.at(i); else return NULL; }

}; 

#endif
