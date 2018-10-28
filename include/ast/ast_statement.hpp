#ifndef ast_statement_hpp
#define ast_statement_hpp

#include <string>
#include <iostream>

extern int scopes;
extern bool ReturnTest;
extern bool main2;
extern std::vector<std::string> functioncalltest;

extern int NoVariables;
extern int LoopsNums;

class Statement : public Ast_Node
{
public:
    virtual ~Statement()
    {}

    //! Tell and expression to print itself to the given stream
    virtual void print(std::ostream &dst) const =0;
	virtual void translate(std::ostream &dst) const =0;
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const =0;

};

class StatementList : public Statement{
	public:
	NodePtr SingleStatement;
    NodePtr RestofStatement;
	
    StatementList(NodePtr _SingleStatement, NodePtr _RestofStatement) :
        SingleStatement(_SingleStatement),
        RestofStatement(_RestofStatement) {}

    ~StatementList()
    {
        delete SingleStatement;
        delete RestofStatement;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst << std::endl;
        SingleStatement->print(dst);
        dst << std::endl;
        RestofStatement->print(dst);
        dst << std::endl;
    }
	virtual void translate(std::ostream &dst) const override{
		
		SingleStatement->translate(dst);
        dst << std::endl;
        RestofStatement->translate(dst);
        dst << std::endl;
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		SingleStatement->print_mips(dst, cont, RegisterLock);
        RestofStatement->print_mips(dst, cont, RegisterLock);

	}
};


class ReturnStatement : public Statement
{
public:
    
	ExpressionPtr expr;
	NodePtr AdditionalStatements;

	~ReturnStatement(){}
	
    ReturnStatement(ExpressionPtr _expr, NodePtr _AdditionalStatements )
	:	expr (_expr)
	,	AdditionalStatements(_AdditionalStatements)
    {ReturnTest = true;}

	
	virtual void print(std::ostream &dst) const override
    {
        dst<< "return ";
		expr->print(dst);
		dst << ";";
    }
	virtual void translate(std::ostream &dst) const override{


		if (expr == NULL){

			for (int i =0; i < scopes; i++){dst << "\t";}
			dst<< "return ";
			AdditionalStatements->translate(dst);
		}
		else {
		
			for (int i =0; i < scopes; i++){dst << "\t";}
			dst<< "return ";
			expr->translate(dst);
		}
	}
    virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		if (AdditionalStatements == NULL){ 
		expr->print_mips(dst, cont, 2);
		dst <<"\tmove\t" << "$sp,$fp" << std::endl;
		dst <<"\tlw\t" << "$31,"<< (NoVariables*4)+8<< "($sp)" << std::endl;
		dst <<"\tlw\t" << "$fp," << (NoVariables*4)+4 << "($sp)" << std::endl;
		dst <<"\taddiu\t" << "$sp,$sp," << (NoVariables*4)+12 << std::endl;
		dst <<"\tj\t" << "$31" << std::endl;
		dst <<"\tnop"<< std::endl;

		}
		else {
			AdditionalStatements->print_mips(dst, cont, 2);
			/*vector<int> free = cont.freeTmpRegisters();
			dst <<"\taddu\t" << "$2" << ",$" << free[0]  << ",$2"<< std::endl;
			*/
		}
		if (main2 == true){
		vector<int> free = cont.freeSavedRegisters();
		dst <<"\taddu\t" << "$2" << ",$" << free[5]  << ",$2"<< std::endl;
			
		dst <<"\tmove\t" << "$sp,$fp" << std::endl;
		dst <<"\tlw\t" << "$31,"<< (NoVariables*4)+8<< "($sp)" << std::endl;
		dst <<"\tlw\t" << "$fp," << (NoVariables*4)+4 << "($sp)" << std::endl;
		dst <<"\taddiu\t" << "$sp,$sp," << (NoVariables*4)+12 << std::endl;
		dst <<"\tj\t" << "$31" << std::endl;
		dst <<"\tnop"<< std::endl;
		}
		else{
		dst <<"\tmove\t" << "$sp,$fp" << std::endl;
		dst <<"\tlw\t" << "$fp," << (NoVariables*4)+8 << "($sp)" << std::endl;
		dst <<"\taddiu\t" << "$sp,$sp," << (NoVariables*4)+12 << std::endl;
		dst <<"\tj\t" << "$31" << std::endl;
		dst <<"\tnop"<< std::endl;
			
		}
		
	}
};

class AssignmentStatement : public Statement
{
public:
    
	std::string Variable;
	ExpressionPtr expr2;

	~AssignmentStatement(){}
	
    AssignmentStatement(std::string _Variable, ExpressionPtr _expr2)
	:	Variable (_Variable),
		expr2 (_expr2)
    {}
	
	virtual void print(std::ostream &dst) const override
    {
        dst << Variable;
		dst << "=";
		expr2->print(dst);
    }
	virtual void translate(std::ostream &dst) const override{
		//std::cout << "The value of scope is: " << scopes << std::endl;
		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << Variable;
		dst << "=";
		expr2->translate(dst);
		dst << std::endl;
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		
		vector<int> free = cont.freeTmpRegisters();
		
		//dst << "\tli\t" << "$" << free[0] << ",";
		expr2->print_mips(dst, cont, free[0]);
		dst << std::endl;

		dst << "\tsw\t$" << free[0]<<  ","<< cont.FindVariable(Variable, scopes) <<"($fp)" << std::endl;
		
	}
    
};

class IfStatement : public Statement
{
public:
	
	ExpressionPtr condition;
	NodePtr body;
  
	~IfStatement(){}
	
	IfStatement(ExpressionPtr _condition, NodePtr _body)
	: condition(_condition), body(_body)
	{}
	
	
	virtual void print(std::ostream &dst) const override
    {
        dst << "if ";
		condition->print(dst);
		dst << "{" <<std::endl;
		body->print(dst);
		dst << "}" <<std::endl;
    }
	virtual void translate(std::ostream &dst) const override{

		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << "if(";
		condition->translate(dst);
		dst << "):" <<std::endl;
		scopes++;
		body->translate(dst);
		scopes--;
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		
		scopes++;
			
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		
		condition->print_mips(dst, cont, free[0]);
		
		dst << "\tbeq\t" << "$0, $" << free[0] << ", $end" << LoopsNums << std::endl;
		dst << "\tnop\t" << std::endl;
		cont.setUnused(free[0]);
		
		if (body != NULL){
		LoopsNums++;
		body->print_mips(dst, cont, RegisterLock);
		LoopsNums--;
		}
		dst << "$end" << LoopsNums << ":" << std::endl;
		//LoopsNums++;
		
		scopes--;

	}
    
};

class IfElseStatement : public Statement
{
public:
	
	ExpressionPtr condition;
	NodePtr body;
	NodePtr body2;
  
	~IfElseStatement(){}
	
	IfElseStatement(ExpressionPtr _condition, NodePtr _body, NodePtr _body2)
	: condition(_condition), body(_body), body2(_body2)
	{}
	
	
	virtual void print(std::ostream &dst) const override
    {
        dst << "if ";
		condition->print(dst);
		dst << "{" <<std::endl;
		body->print(dst);
		dst << "}" <<std::endl;
		dst << "else {";
		body2->print(dst);
		dst << "}";
    }
	virtual void translate(std::ostream &dst) const override{
		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << "if(";
		condition->translate(dst);
		dst << "):" <<std::endl;
		scopes++;
		body->translate(dst);
		scopes--;
		dst <<std::endl;
		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << "else:";
		dst <<std::endl;
		scopes++;
		body2->translate(dst);
		scopes--;
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
					
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		
		condition->print_mips(dst, cont, free[0]);
		
		dst << "\tbeq\t" << "$0, $" << free[0] << ", $else" << LoopsNums << std::endl;
		dst << "\tnop\t" << std::endl;
		cont.setUnused(free[0]);

		if (body != NULL){
		LoopsNums++;
		body->print_mips(dst, cont, RegisterLock);
		LoopsNums--;
		}
		dst << "\tb\t" << "$end" << LoopsNums << std::endl;
		
		dst << "$else" << LoopsNums << ":" << std::endl;
		if (body2 != NULL){
		LoopsNums++;
		body2->print_mips(dst, cont, RegisterLock);
		LoopsNums--;
		}
	
		dst << "$end" << LoopsNums << ":" << std::endl;
		LoopsNums++;
	}
		
    
};

class WhileStatement : public Statement
{
public:
	
	ExpressionPtr condition;
	NodePtr body;
  
	~WhileStatement(){}
	
	WhileStatement(ExpressionPtr _condition, NodePtr _body)
	: condition(_condition), body(_body)
	{}
	
	
	virtual void print(std::ostream &dst) const override
    {
        dst << "while ";
		condition->print(dst);
		dst << "{" <<std::endl;
		body->print(dst);
		dst << "}" <<std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override{
		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << "while(";
		condition->translate(dst);
		dst << "):" <<std::endl;
		scopes++;
		body->translate(dst);
		scopes--;

		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		
		dst << "$Top" << LoopsNums << ":" << std::endl;
		
		condition->print_mips(dst, cont, free[0]);
		
		dst << "\tbeq\t" << "$0, $" << free[0] << ", $end" << (LoopsNums+1) << std::endl;
		dst << "\tnop\t" << std::endl;

		if (body != NULL){
		LoopsNums++;
		body->print_mips(dst, cont, RegisterLock);
		LoopsNums--;
		}
		
		dst << "\tb\t" << "$Top" << LoopsNums << std::endl;
		dst << "nop" << std::endl;
		LoopsNums++;
		dst << "$end" << LoopsNums << ":" << std::endl;
		
		cont.setUnused(free[0]);
	}
    
};

class DoWhileStatement : public Statement
{
public:
	
	ExpressionPtr condition;
	NodePtr body;
  
	~DoWhileStatement(){}
	
	DoWhileStatement(ExpressionPtr _condition, NodePtr _body)
	: condition(_condition), body(_body)
	{}
	
	
	virtual void print(std::ostream &dst) const override
    {
		dst << "do {" << std::endl;
		body->print(dst);
		dst << "}" << "while ( ";
		condition->print(dst);
		dst << ")" <<std::endl;

    }
	
	virtual void translate(std::ostream &dst) const override{
		// Not Implemented
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		
		dst << "$do" << LoopsNums << ":" << std::endl;
		
		if (body != NULL){
		LoopsNums++;
		body->print_mips(dst, cont, RegisterLock);
		LoopsNums--;
		}
		
		condition->print_mips(dst, cont, free[0]);
		
		dst << "\tbeq\t" << "$0, $" << free[0] << ", $end" << (LoopsNums+1) << std::endl;
		dst << "\tnop\t" << std::endl;

		dst << "\tb\t" << "$do" << LoopsNums << std::endl;
		dst << "nop" << std::endl;
					
		dst << "$end" << LoopsNums << ":" << std::endl;
		//LoopsNums++;
		cont.setUnused(free[0]);
	}
    
};

class ForStatement : public Statement
{
public:
	NodePtr Variable;
	ExpressionPtr Condition;
	NodePtr Step;
	NodePtr body;
  
	~ForStatement(){}
	
	ForStatement(NodePtr _Variable, ExpressionPtr _Condition, NodePtr _Step, NodePtr _body)
	: Variable (_Variable), Condition(_Condition), Step(_Step), body(_body) {}
	
	
	virtual void print(std::ostream &dst) const override
    {
        dst << "For( ";
		Variable->print(dst);
		dst << ";";
		Condition->print(dst);
		dst << ";";
		Step->print(dst);
		dst << "){" << std::endl;
		body->print(dst);
		dst << "}" <<std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override{
		//No Implementation Needed
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		
		//dst << "\tInitial Variable Function : \t" << std::endl;
		Variable->print_mips(dst, cont, RegisterLock);
		
		dst << "$Top" << LoopsNums << ":" << std::endl;
		
		Condition->print_mips(dst, cont, free[0]);
		
		dst << "\tbeq\t" << "$0, $" << free[0] << ", $end" << LoopsNums << std::endl;
		dst << "\tnop\t" << std::endl;
		
		//dst << "\tBody Function : \t" << std::endl;

			
		if (body != NULL){
		LoopsNums++;
		body->print_mips(dst, cont, RegisterLock);
		LoopsNums--;
		}
		//dst << "\tStep Function : \t" << std::endl;

		
		Step->print_mips(dst, cont, RegisterLock);
		
		dst << "\tb\t" << "$Top" << LoopsNums << std::endl;
		
		dst << "nop" << std::endl;
					
		dst << "$end" << LoopsNums << ":" << std::endl;
		LoopsNums++;
		cont.setUnused(free[0]);
	}
    
};

class FunctionCallStatement : public Statement
{
public:
	
	std::string name;
	NodePtr body;
	~FunctionCallStatement(){}
	
	FunctionCallStatement(std::string _name, NodePtr _body)
	: name(_name)
	, body(_body)	
	{functioncalltest.push_back(name);}
	
	virtual void print(std::ostream &dst) const override
    {
        dst<<name<< "(";
		if (body != NULL){
			body->print(dst);
		}
		dst<< ")";
    }
	
	virtual void translate(std::ostream &dst) const override{
		
		dst<<name<< "(";
		if (body != NULL){
			body->translate(dst);
		}
		dst<<")";
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		if (body != NULL){
			body->print_mips(dst, cont, RegisterLock);
		}
		dst <<"\t.option pic0 \t" << std::endl;
		dst <<"\tjal\t" << name << std::endl;
		dst <<"\tnop"<< std::endl;
		dst <<"\t.option pic2 \t" << std::endl;
		
		cont.freeUpArgumentRegisters();
		
		//Move Final Value back into Register 2
		//sw	$2,28($fp)

	}
    
};

class FunctionCallStatement2 : public Statement
{
public:
	std::string VariableName;
	std::string functionName;
	NodePtr body;
	
	~FunctionCallStatement2(){}
	
	FunctionCallStatement2(std::string _VariableName,std::string _functionName, NodePtr _body)
	: VariableName(_VariableName)
	, functionName(_functionName)
	, body(_body)	
	{functioncalltest.push_back(functionName);}
	
	virtual void print(std::ostream &dst) const override
    {
        dst<<VariableName<< "=" << functionName << "(";
		if (body != NULL){
			body->print(dst);
		}
		dst<< ");";
    }
	
	virtual void translate(std::ostream &dst) const override{
		
		 dst<<VariableName<< "=" << functionName << "(";
		if (body != NULL){
			body->translate(dst);
		}
		dst<< ");";
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		if (body != NULL){
			body->print_mips(dst, cont, RegisterLock);
		}
		dst <<"\t.option pic0 \t" << std::endl;
		dst <<"\tjal\t" << functionName << std::endl;
		dst <<"\tnop"<< std::endl;
		dst << std::endl;
		dst <<"\t.option pic2 \t" << std::endl;
		
		//Move Final Value back into Register 2
		cont.freeUpArgumentRegisters();
		
		
		dst <<"\tsw\t" << "$2," << cont.FindVariable(VariableName, scopes) << "($fp)" << std::endl;
		
	}
    
};
class ArrayDeclaration : public Statement
{
public:
	std::string arrayname;
	int arraysize;

  
	~ArrayDeclaration(){}
	
	ArrayDeclaration(std::string _arrayname, int _arraysize)
	: arrayname (_arrayname), arraysize(_arraysize) 
	{NoVariables = NoVariables + arraysize;}
	
	
	
	virtual void print(std::ostream &dst) const override
    {
	dst << "int " << arrayname << "[" << arraysize << "];" << std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override{
		//No Implementation Needed
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		dst <<"\t.data\t" << arrayname << std::endl;
		dst <<"array:\t.word\t"<< "0:" << arraysize<<  std::endl;
		
	}
    
};

class Label : public Statement
{
public:
	std::string name;

  
	~Label(){}
	
	Label(std::string _name)
	: name (_name)
	{}
	
	
	
	virtual void print(std::ostream &dst) const override
    {
	dst << name <<  " :" << std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override{
		//No Implementation Needed
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		dst << name <<"\t"<< ":" <<  std::endl;
		
	}
    
};

class Continue : public Statement
{
public:
	std::string name;

  
	~Continue(){}
	
	Continue(){}
	
	virtual void print(std::ostream &dst) const override
    {
	dst << "continue" << std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override{
		//No Implementation Needed
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		dst << "\tb\t" << "$Top" << LoopsNums << std::endl;
	}
    
};


class Break : public Statement
{
public:
	  
	~Break(){}
	
	Break(){}
	
	virtual void print(std::ostream &dst) const override
    {
	dst << "break" << std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override{
		//No Implementation Needed
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		dst << "\tb\t" << "$end" << LoopsNums << std::endl;
		
	}
    
};

class Jump : public Statement
{
public:
	 std::string name;
 
	~Jump(){}
	
	Jump(std::string _name)
	: name (_name)
	{} 
	
	virtual void print(std::ostream &dst) const override
    {
	dst << "break" << std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override{
		//No Implementation Needed
		
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		dst << "\tb\t" << "$" << name << std::endl;
		
	}
    
};
#endif
