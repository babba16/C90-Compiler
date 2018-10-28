#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"

#include <string>
#include <iostream>
#include <map>

#include <memory>

class Expression;

typedef const Expression *ExpressionPtr;
class Expression : public Ast_Node
{
public:
    virtual ~Expression()
    {}

    //! Tell and expression to print itself to the given stream
    virtual void print(std::ostream &dst) const =0;
	virtual void translate(std::ostream &dst) const =0;
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const =0;
};

class SpecialSubOperator : public Expression
{

public:
	ExpressionPtr Variable1;
	int Variable2;
	
    SpecialSubOperator(ExpressionPtr _Variable1, int _Variable2)
	: Variable1(_Variable1),
	  Variable2(_Variable2)
    {}
    
	virtual void print(std::ostream &dst) const override
    {
        Variable1->print(dst);
		dst << Variable2;
    }
	virtual void translate(std::ostream &dst) const override{
		
		Variable1->translate(dst);
		dst << Variable2;
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		Variable1->print_mips(dst, cont, RegisterLock);
	
		dst << "\tadd\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ", "<< Variable2<< std::endl;
		
	}
   
};

#endif
