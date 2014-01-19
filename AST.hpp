#ifndef AST_HPP
#define AST_HPP

enum AstID { 
	BaseID, 
	VariableDeclID, 
	BinaryExprID, 
	CallExprID, 
	JumpStmtID, 
	VariableID, 
	NumberID
}; 

class BaseAst { 
	private:
		AstID ID; 

	public:
		BaseAST(AstID id) : ID(id) { }
		virtual ~BaseAST() { }
		AstID getValueID() const { return ID; }

}; 

class VariableDeclAST: public BaseAST { 
	public:
		typedef enum { 
			param, 
			local
		}DeclType; 

	private:
		std::string Name; 
		DeclType Type; 
	
	public:
		VariableDeclAST(const std::string& name) : BaseAST(VariableDeclID), Name(name) { }

		static inline bool classof(VariableDeclAST const * ) { return true; }

		static inline bool classof(BaseAST const * base) { 
			return base->getValueID() == VariableDeclID; 
		}
		~VariableDeclAST() { }

		std::string getName() { return Name; }
		
		bool setDeclType(DeclType type) { Type = type; return true; }; 

		DeclType getType() { return Type; }
}; 

class BinaryExprAST : public BaseAST { 
	private:
		std::string Op; 
		BaseAST  * LHS,  * RHS; 

	public:
		BinaryExprAST(std::string op, BaseAST * lhs, BaseAST * rhs) : BaseAST(BinaryExprID), Op(op), LHS(lhs), RHS(rhs) { }
		~BinaryExprAST() { SAFE_DELETE(LHS); SAFE_DELETE(RHS); }

		static inline bool classof(BinaryExprAST const * ) { return true; }

		static inline bool classof(BaseAST const * base) { 
			return base->getValueID() == BinaryExprID; 
		}

		std::string getOp() { return Op; }

		BaseAST * getLHS() { return LHS; }

		BaseAST * getRHS() { return RHS; }

}; 

class CallExprAST : public BaseAST { 
	private:
		std::string Callee; 
		std::vector<BaseAST * > Args; 

	public:
		CallExprAST(const std::string& callee, std::vector<BaseAST * >& args) : BaseAST(CallExprID), Callee(callee), Args(args) { }
		~CallExprAST(); 

		static inline bool classof(CallExprAST const * ) { return true; }

		static inline bool classof(BaseAST const * base) { 
			return base->getValueID() == CallExprID; 
		}

		std::string getCallee() { return Callee; }

		BaseAst * getArgs(int i) { if(i < Arg.size())return Args.at(i); else return NULL; }

}; 

class JumStmtAST : public BaseAST { 
	private:
		BaseAST * expr; 
	
	public:
		JumpStmtAST(BaseAST * expr) : BaseAST(JumpStmtID), Expr(expr) { }
		~JumpStmtAST() { SAFE_DELETE(Expr); }

		static inline bool classof(JumpStmtAST const * ) { return true; }

		static inline bool classsof(BaseAST const * base) { 
			return base->getValueID() == JumpStmtID; 
		}

		BaseAST * getExpr() { return Expr; }

}; 

class VariableAST : public BaseAST { 
	private:
		std::string Name; 

	public:
		VariableAST(const std::string &name) : BaseAST(VariableID), Name(name) { }
		~VariableAST() { }

		static inline bool classof(VariableAST const * ) { return true; }

		static inline bool classof(BaseAST const * base) { 
			return base->getValueID() == VariableID; 
		}
		
		std::string getName() { return Name; }
}; 

class NumberAST : public BaseAST { 
	private:
		int Val; 

	public:
		NumberAST(int val) : BaseAST(NumberID), Val(val) { }
		~NumberAST() { }

		static inline bool classof(NumberAST const * ) { return true; }

		static inline bool classof(BaseAST const * base) { 
			return base->getValueID() == NumberID; 
		}

		int getNumberValue() { return Val; }
}; 
			
#endif
