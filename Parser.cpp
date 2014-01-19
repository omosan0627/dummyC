#include "PARSER.hpp"
#include "TOKENSTREAM.hpp"
#include <iostream>
#include <string>
#include <vector>

Parser::Parser(std::string filename) { 
	Tokens = LexicalAnalysis(filename); 
}

bool Parser::doParse() { 
	if(!Tokens) { 
		std::cerr << "error at lexer" << std::endl; 
		return false; 
	}
	else { 
		return visitTranslationUnit(); 
	}
}

TranslationUnitAST &Parser::getAST() { 
	if(TU)return * TU; 
	else return  * (new TranslationUnitAST()); 
}

bool Parser::visitTranslationUnit() { 
	TU = new TranslationUnitAST(); 

	while(true) { 
		if(!visitExternalDeclaration(TU)) { 
			SAFE_DELETE(TU); 
			return false; 
		}
		if(Tokens->getCurType() == TOK_EOF)break; 
	}
	return true; 
}

bool Parser::visitExternalDeclaration(TranslationUnitAST * tunit) { 
	ProtoTypeAST * proto = visitFunctionDeclearation(); 
	if(proto) { 
		tunit->addPrototype(proto); 
		std::cout << "ProtoType" << std::endl; 
		return true; 
	}

	std::cout << "ProtoTypeFail" << std::endl; 

	FunctionAST * func_def = visitFunctionDefinition(); 
	if(func_def) { 
		tunit->addFunction(func_def); 
		std::cout << "Function" << std::endl; 
		return true; 
	}
	std::cout << "Nothing" << std::endl; 
	return false; 
}

ProtoTypeAST * Parser::visitFunctionDeclearation() { 
	int bkup = Tokens->getCurIndex(); 
	ProtoTypeAST * proto = visitPrototype(); 
	if(!proto) { 
		std::cout << "visitPrototype() failed" << std::endl; 
		return NULL; 
	}
	std::cout << "visitPrototype() passed" << std::endl; 

	if(Tokens->getCurString() == ";") { 
		Tokens->getNextToken(); 
		return proto; 
	}
	else { 
		SAFE_DELETE(proto); 
		Tokens->applyTokenIndex(bkup); 
		return NULL; 
	}
}

FunctionAST * Parser::visitFunctionDefinition() { 
	int bkup = Tokens->getCurIndex(); 
	ProtoTypeAST * proto = visitPrototype(); 
	if(!proto) { 
		std::cout << "visitPrototype() failed" << std::endl; 
		return NULL; 
	}
	std::cout << "visitPrototype() passed" << std::endl; 

	FunctionStmtAST * func_stmt = visitFunctionStatement(proto); 
    if(func_stmt){
        return new FunctionAST(proto,func_stmt);
    }
	else{
        SAFE_DELETE(proto);
        Tokens->applyTokenIndex(bkup);
        return NULL;
    }	
}


ProtoTypeAST * Parser::visitPrototype(){
    int bkup=Tokens->getCurIndex();
	std::string func_name; 

    if(Tokens->getCurType()==TOK_INT){
		std::cout << "int " << std::endl; 
        Tokens->getNextToken();
    }
	else{
        return NULL;
    }

    if(Tokens->getCurType()==TOK_IDENTIFIER){
		std::cout << "identifier " << std::endl; 
        Tokens->getNextToken();
		func_name=Tokens->getCurString();
    }
	else{
        Tokens->ungetToken(1);
        return NULL;
    }

    if(Tokens->getCurString()=="("){
		std::cout << "( " << std::endl; 
        Tokens->getNextToken();
    }
	else{
        Tokens->ungetToken(2);
        return NULL;
    }

    std::vector<std::string> param_list;
    bool is_first_param = true;
    while(true){
        if(!is_first_param && Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()==","){
            Tokens->getNextToken();
        }
        if(Tokens->getCurType()==TOK_INT){
            Tokens->getNextToken();
        }
		else break;

		std::cout << "paramend" << std::endl;

		if(Tokens->getCurType()==TOK_IDENTIFIER){
			param_list.push_back(Tokens->getCurString());
			Tokens->getNextToken();
		}
		else{
			Tokens->applyTokenIndex(bkup);
			return NULL;
		}
		is_first_param = false; 
	}
	
	if(Tokens->getCurString()==")"){
		Tokens->getNextToken();
		return new ProtoTypeAST(func_name, param_list);
	}
	else{
        Tokens->applyTokenIndex(bkup);
        return NULL;
    }
}

FunctionStmtAST * Parser::visitFunctionStatement(ProtoTypeAST *proto){
    int bkup=Tokens->getCurIndex();

     if(Tokens->getCurString()=="{"){
            Tokens->getNextToken();
     }
	else{
		return NULL;
    }

	FunctionStmtAST *func_stmt = new FunctionStmtAST();

    for(int i=0; i<proto->getParamNum(); i++){
        VariableDeclAST *vdecl=new VariableDeclAST(proto->getParamName(i));
        vdecl->setDeclType(VariableDeclAST::param);
        func_stmt->addVariableDeclaration(vdecl);
    }


    VariableDeclAST *var_decl;
	BaseAST *stmt;
    BaseAST *last_stmt;

    if(stmt=visitStatement()){
        while(stmt){
            last_stmt=stmt;
            func_stmt->addStatement(stmt);
            stmt=visitStatement();
         }
    }
	else if(var_decl=visitVariableDeclaration()){
        while(var_decl){
            var_decl->setDeclType(VariableDeclAST::local);
			func_stmt->addVariableDeclaration(var_decl);
            var_decl=visitVariableDeclaration();
		}
        if(stmt=visitStatement()){
             while(stmt){
                last_stmt=stmt;
                func_stmt->addStatement(stmt);
                stmt=visitStatement();
            }
        }
	}
	else{
		SAFE_DELETE(func_stmt);
        Tokens->applyTokenIndex(bkup);
        return NULL;
    }

    if(Tokens->getCurString()=="}"){
        Tokens->getNextToken();
        return func_stmt;
    }
	else{
        SAFE_DELETE(func_stmt);
        Tokens->applyTokenIndex(bkup);
        return NULL;
    }
}


VariableDeclAST *Parser::visitVariableDeclaration(){
    std::string name;
    if(Tokens->getCurType()==TOK_INT){
        Tokens->getNextToken();        
    }
	else{
        return NULL;
    }
	if(Tokens->getCurType()==TOK_IDENTIFIER){
		name=Tokens->getCurString();
        Tokens->getNextToken();
    }
	else{
        Tokens->ungetToken(1);
        return NULL;
    }
    if(Tokens->getCurString()==";"){
        Tokens->getNextToken();
        return new VariableDeclAST(name);
    }
	else{
        Tokens->ungetToken(2);        
        return NULL;
    }
}


BaseAST *Parser::visitStatement(){
    BaseAST *stmt=NULL;
    if(stmt=visitExpressionStatement()){
        return stmt;
    }
	else if(stmt=visitJumpStatement()){
        return stmt;
    }
	else{
        return NULL;
    }
}

BaseAST *Parser::visitExpressionStatement(){
    BaseAST *assign_expr;
    if(Tokens->getCurString()==";"){
        Tokens->getNextToken();
        return new NullExprAST();
    }
	else if((assign_expr=visitAssignmentExpression())){
        if(Tokens->getCurString()==";"){
            Tokens->getNextToken();
            return assign_expr;
        }
    }
    return NULL;
}


BaseAST *Parser::visitJumpStatement(){
	int bkup=Tokens->getCurIndex();
    BaseAST *expr;
	if(Tokens->getCurType() == TOK_RETURN){
        Tokens->getNextToken();        
        if(!(expr=visitAssignmentExpression()) ){
            Tokens->applyTokenIndex(bkup);
            return NULL;
        }
		if(Tokens->getCurString()==";"){
            Tokens->getNextToken();
            return new JumpStmtAST(expr);
        }
		else{
            Tokens->applyTokenIndex(bkup);
            return NULL;
		}
    }
	else{
        return NULL;
    }
}


BaseAST * Parser::visitAssignmentExpression(){
    int bkup=Tokens->getCurIndex();
    BaseAST *lhs;

    if(Tokens->getCurType()==TOK_IDENTIFIER){
		lhs=new VariableAST(Tokens->getCurString());
        Tokens->getNextToken();
		BaseAST *rhs;
        if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()=="="){
            Tokens->getNextToken();
            if(rhs=visitAdditiveExpression(NULL)){
                return new BinaryExprAST("=", lhs, rhs);
            }
			else{
                SAFE_DELETE(lhs);
                Tokens->applyTokenIndex(bkup);
            }
        }
		else{
            SAFE_DELETE(lhs);
			Tokens->applyTokenIndex(bkup);
        }
    }   
	BaseAST *add_expr=visitAdditiveExpression(NULL);
    if(add_expr){
        return add_expr;
    }
	return NULL;
}


BaseAST *Parser::visitPrimaryExpression(){
	int bkup=Tokens->getCurIndex();
    if(Tokens->getCurType()==TOK_IDENTIFIER){
        std::string var_name=Tokens->getCurString();
        Tokens->getNextToken();
        return new VariableAST(var_name);
	}
	else if(Tokens->getCurType()==TOK_DIGIT){
        int val=Tokens->getCurNumVal();
        Tokens->getNextToken();
        return new NumberAST(val);
    }
	else if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()=="-"){
        Tokens->getNextToken();
        if(Tokens->getCurType()==TOK_DIGIT){
            int val=Tokens->getCurNumVal();
            Tokens->getNextToken();
            return new NumberAST(-val);
        }
		else{
            Tokens->applyTokenIndex(bkup);
            return NULL;
        }
    }
	else if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()=="("){
        Tokens->getNextToken();
        BaseAST *assign_expr;
        if(!(assign_expr=visitAssignmentExpression())){
            Tokens->applyTokenIndex(bkup);
            return NULL;
        }
        if(Tokens->getCurString()==")"){
            Tokens->getNextToken();
            return assign_expr;
        }
		else{
            SAFE_DELETE(assign_expr);
            Tokens->applyTokenIndex(bkup);
            return NULL;
        }
	}
	return NULL;
}

BaseAST *Parser::visitPostfixExpression(){
    int bkup=Tokens->getCurIndex();
    BaseAST *prim_expr=visitPrimaryExpression();
    if(prim_expr){
        return prim_expr;
    }
    if(Tokens->getCurType()==TOK_IDENTIFIER) {   
        std::string Callee=Tokens->getCurString();
        Tokens->getNextToken();
        if(Tokens->getCurType()!=TOK_SYMBOL || Tokens->getCurString()!="("){
            Tokens->applyTokenIndex(bkup);
            return NULL;
		}

		Tokens->getNextToken();
        std::vector<BaseAST*> args;
        BaseAST *assign_expr=visitAssignmentExpression();

        if(assign_expr){
            args.push_back(assign_expr);
            while(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()==","){
                Tokens->getNextToken();
                assign_expr=visitAssignmentExpression();
                if(assign_expr){
                        args.push_back(assign_expr);
                }
				else{
                    break;
                }
			}
		}

		if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()==")"){
            Tokens->getNextToken();
            return new CallExprAST(Callee, args);
        }
		else{
            for(int i=0;i<args.size();i++) SAFE_DELETE(args[i]);
            Tokens->applyTokenIndex(bkup);
            return NULL;
        }
	}
	else{
		return NULL;
    }
}

BaseAST *Parser::visitAdditiveExpression(BaseAST *lhs){
    int bkup=Tokens->getCurIndex();
	if(!lhs) lhs=visitMultiplicativeExpression(NULL);
    
	BaseAST *rhs;
	if(!lhs){
        return NULL;
    }
    if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()=="+"){
        Tokens->getNextToken();
        rhs=visitMultiplicativeExpression(NULL);
        if(rhs){
            return visitAdditiveExpression(new BinaryExprAST("+", lhs, rhs));
        }
		else{
            SAFE_DELETE(lhs);
            Tokens->applyTokenIndex(bkup);
            return NULL;
		}
    }
	else if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()=="-"){
        Tokens->getNextToken();
		rhs=visitMultiplicativeExpression(NULL);
        if(rhs){
            return visitAdditiveExpression(new BinaryExprAST("-", lhs, rhs));
        }
		else{
			SAFE_DELETE(lhs);
            Tokens->applyTokenIndex(bkup);
            return NULL;
        }        
    }        
	return lhs;
}


BaseAST * Parser::visitMultiplicativeExpression(BaseAST *lhs){
    int bkup=Tokens->getCurIndex();
    if(!lhs) lhs=visitPostfixExpression();
    BaseAST *rhs;
	if(!lhs){
        return NULL;
    }
    if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()=="*"){
        Tokens->getNextToken();
        rhs=visitPostfixExpression();
        if(rhs){
            return visitMultiplicativeExpression(new BinaryExprAST("*", lhs, rhs));
        }
		else{
            SAFE_DELETE(lhs);
            Tokens->applyTokenIndex(bkup);
            return NULL;
		}
    }
	else if(Tokens->getCurType()==TOK_SYMBOL && Tokens->getCurString()=="/"){
        Tokens->getNextToken();
        rhs=visitPostfixExpression();
        if(rhs){
            return visitMultiplicativeExpression(new BinaryExprAST("/", lhs, rhs));
        }
		else{
            SAFE_DELETE(lhs);
            Tokens->applyTokenIndex(bkup);
            return NULL;
        }        
	}        
    return lhs;
}
