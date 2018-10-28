#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include <string>
#include <vector>
#include <iostream>
#include "ast_node.hpp"

extern int LoopsNums;

extern std::vector<std::string> globals;
extern std::vector<std::string> functioncalltest;

extern int scopes;

extern int NoVariables;


extern bool ReturnTest;

extern bool main2;

class Declaration : public Ast_Node
{
public:
    virtual ~Declaration()
    {}
	
	virtual void translate(std::ostream &dst) const =0;

    //! Tell and expression to print itself to the given stream
    virtual void print(std::ostream &dst) const =0;
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const =0;

};

// GLOBALS VARIABLES
class VariableDeclaration : public Declaration
{
public:
    std::string Type;
    std::string Variable1;
	ExpressionPtr Variable2;

	~VariableDeclaration(){}
	
    VariableDeclaration(std::string& _Type, std::string& _Variable1, ExpressionPtr _Variable2)
        : Type(_Type)
        , Variable1(_Variable1)
		, Variable2 (_Variable2)
			{ globals.push_back(Variable1);	
			NoVariables++;
			}
	
	
	virtual void print(std::ostream &dst) const override
    {
         dst<< Type <<" " <<Variable1;
		 if (Variable2 != NULL){
			 dst <<" = ";
			 Variable2->print(dst);
		 }
		 dst << ";";
    }
	
	virtual void translate(std::ostream &dst) const override
    {
         dst<<Variable1;
		 if (Variable2 != NULL){
			 dst <<"=";
			 Variable2->translate(dst);
			 dst << "";
		 }
		 else {
		 dst << "=0";
    }
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeSavedRegisters();
		
		if (Variable2 != NULL){
			
		
		dst << "\t.globl\t" << Variable1 << std::endl;
		dst << "\t.data\t" <<  std::endl;
		dst << "\t.align\t2" << std::endl;
		
		dst << Variable1 << ":" << std::endl;
		dst << "\t.word\t"; 
		Variable2->print(dst);
		dst << std::endl;
		dst << "\t.text" << std::endl; 
		dst << "\t.align\t2" << std::endl;
		
		cont.AddGlobal(Variable1);
		
		}
		
		if (Variable2 == NULL){
			//Variable declared but not assigned - needs to be added to the map
			cont.AddGlobal(Variable1);
		}
	}
    
};

//LocalVariableDeclaration
class VariableDeclaration2 : public Declaration
{
public:
    std::string Type;
    std::string Variable1;
	ExpressionPtr Variable2;
	bool ParameterTest;

	~VariableDeclaration2(){}
	
    VariableDeclaration2(std::string& _Type, std::string& _Variable1, ExpressionPtr _Variable2, bool _ParameterTest)
        : Type(_Type)
        , Variable1(_Variable1)
		, Variable2 (_Variable2)
		, ParameterTest (_ParameterTest)
			{NoVariables++;}
	
	
	virtual void print(std::ostream &dst) const override
    {
         dst<< Type <<" " <<Variable1;
		 if (Variable2 != NULL){
			 dst <<"=";
			 Variable2->print(dst);
		 }
		 dst << ";";
    }
	
	virtual void translate(std::ostream &dst) const override
    {
		if (ParameterTest == true)
			{
			dst<<Variable1;	
			}
		else {
			dst<<"\t"<<Variable1;	
		}
		
         
		if (Variable2 != NULL){
			 dst <<" = ";
			 Variable2->translate(dst);
		 }
		 
		 else { 
			if (ParameterTest == true)
			{
			}
			else{
			dst << "=0";}
		 }
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		
		vector<int> free = cont.freeTmpRegisters();
		
		//dst << Variable1 + std::to_string(scopes) << std::endl;
		// Variable Declaration for a single value expression
		if (Variable2 != NULL){
		Variable2->print_mips(dst, cont, free[0]);
		cont.AddVariable(Variable1 + std::to_string(scopes));
		dst << "\tsw\t$" << free[0] << ", " << cont.FindVariable(Variable1, scopes) <<"($fp)" << std::endl;
		}
		else{
				if (ParameterTest == true){
		
			free = cont.freeArgumentRegisters();
			cont.setUsed(free[0]);
			//Variable declared but not assigned - needs to be added to the map
			// Same Declaration for Parameter
			cont.AddVariable(Variable1 + std::to_string(scopes));
			dst << "\tsw\t$" << free[0] << ", " << cont.FindVariable(Variable1, scopes) <<"($fp)" << std::endl;
			
			}
			else {
				cont.AddVariable(Variable1 + std::to_string(scopes));
			}

	}
	}
    
};

class FunctionDefinition
    : public Declaration
{
	
public:
    std::string Type;
    std::string Name;

	~FunctionDefinition(){}
  FunctionDefinition(std::string _Type,  std::string _Name)
        : Type(_Type)
        , Name(_Name)
    {}
	
    virtual void print(std::ostream &dst) const override
    {
         dst<<Type;
        dst<<" ";
		dst << Name;
        dst<<"();" << std::endl;
    }
	
	virtual void translate(std::ostream &dst) const override
    {
       ;
    }
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {}
    
};

class FunctionDeclaration
    : public Declaration
{
	
public:
    std::string Type;
    std::string Name;
	NodePtr Parameter;
	NodePtr Body;

	~FunctionDeclaration(){}
   FunctionDeclaration(std::string _Type,  std::string _Name, NodePtr _Parameter, NodePtr _Body)
        : Type(_Type)
        , Name(_Name)
		, Parameter (_Parameter)
		, Body (_Body)
		{}
   
  
	
    virtual void print(std::ostream &dst) const override
    {

        dst<<Type;
        dst<<" ";
		dst << Name;
		dst << "(";
		if (Parameter != NULL){
			Parameter->print(dst);
		}
		dst << ")";
        dst<<" {" << std::endl;
		if (Body!= NULL){
			Body->print(dst);
			dst << std::endl;
		}
		dst<<" }" << std::endl;
    }

	virtual void translate(std::ostream &dst) const override
    {
		scopes++;
		//std::cout << "The value of scope FunctionStart: " << scopes << std::endl;
		if (Name!= "main"){
		dst<<"def ";
		dst << Name;
		dst << "(";
		if (Parameter != NULL){
			Parameter->translate(dst);
		}
		dst << "):";
        dst<<std::endl;
		for (int i = 0; i < globals.size(); i++){
			dst << "\t" <<"global ";
			dst << globals[i];
			dst <<std::endl;
		}
        if (Body!= NULL){
			Body->translate(dst);
		}
		dst << std::endl;
		scopes--;

		}
		
		else {
			dst<<"def ";
		dst << Name;
		dst << "(";
		if (Parameter != NULL){
			Parameter->translate(dst);
		}
		dst << "):";
        dst<<std::endl;
        if (Body!= NULL){
			Body->translate(dst);
		}
		dst << std::endl;
		
		dst << "if" << " __name__" << " == " << "\"__main__\":" << std::endl;
		dst << "\timport sys" << std::endl;
		dst << "\tret=main()" << std::endl;
		dst << "\tsys.exit(ret)" << std::endl;
		scopes--;

		
		}
        
    }
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		 main2 = true;
		
		for(int i = 0; i < functioncalltest.size() ; i++) {
			if (functioncalltest[i] ==  Name){ main2 = false;}
			if (functioncalltest[0] == Name && functioncalltest.size() ==1){main2 = true;}
		}
		dst <<"\t.global\t" << Name << std::endl;
		dst <<"\t.ent\t" << Name << std::endl;
		//Create a label
		dst << Name << ":" << std::endl;
		dst <<"\taddiu\t" << "$sp,$sp,-" << (NoVariables*4)+12 << std::endl;
		if (main2 == true){
		dst <<"\tsw\t" << "$31,"<< (NoVariables*4)+8<< "($sp)" << std::endl;
		dst <<"\tsw\t" << "$fp,"<< (NoVariables*4)+4<< "($sp)" << std::endl;
		}
		else {
		dst <<"\tsw\t" << "$fp,"<< (NoVariables*4)+8<< "($sp)" << std::endl;
		}
		dst <<"\tmove\t" << "$fp,$sp" << std::endl;
		if (Parameter != NULL){
			Parameter->print_mips(dst, cont, RegisterLock);
		}
		cont.freeUpArgumentRegisters();
		
		if (Body != NULL){
			Body->print_mips(dst, cont, RegisterLock);
		}

		
		if (ReturnTest!= true){
			dst <<"\tmove\t" << "$2,$0" << std::endl;
		}
		dst <<"\tmove\t" << "$sp,$fp" << std::endl;
		
		if (main2 == true){
		dst <<"\tlw\t" << "$31,"<< (NoVariables*4)+8<< "($sp)" << std::endl;
		dst <<"\tlw\t" << "$fp," << (NoVariables*4)+4 << "($sp)" << std::endl;
		}
		else {
		dst <<"\tlw\t" << "$fp," << (NoVariables*4)+8 << "($sp)" << std::endl;
		}		
		dst <<"\taddiu\t" << "$sp,$sp," << (NoVariables*4)+12 << std::endl;
		dst <<"\tj\t" << "$31" << std::endl;
		dst <<"\tnop"<< std::endl;
		
		dst << std::endl;
		dst <<"\t.end\t" << Name << std::endl;
		
		main2 = false;
		
		cont.freeUpArgumentRegisters();
		
		LoopsNums =LoopsNums + 10;
		

		
	}
    
};
	
	
	
#endif