#ifndef ast_parameter_hpp
#define ast_parameter_hpp

#include <string>
#include <iostream>
#include <vector>


class Parameter : public Ast_Node
{
public:

    virtual ~Parameter()
    {}

    //! Tell and expression to print itself to the given stream
    virtual void print(std::ostream &dst) const =0;
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const =0;

};

class ParameterDef : public Parameter
{
public:
	ExpressionPtr Variable2;
	
	virtual ~ParameterDef()
    {}

	
    ParameterDef(ExpressionPtr _Variable2)
		: Variable2 (_Variable2)
		{NoVariables++;}
	
	virtual void print(std::ostream &dst) const override
    {
          Variable2->print(dst);
    }
	
	virtual void translate(std::ostream &dst) const override
    {
         Variable2->translate(dst);
    }
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
			
			vector<int> free = cont.freeArgumentRegisters();
			cont.setUsed(free[0]);
			Variable2->print_mips(dst, cont, free[0]);
	}


};
class ReturnList : public Ast_Node{
	public:
	NodePtr left;
    NodePtr right;

	
    ReturnList(NodePtr _left, NodePtr _right) :
        left(_left),
        right(_right) {}

    ~ReturnList()
    {
        delete left;
        delete right;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst << std::endl;
        left->print(dst);
        dst << " + ";
        right->print(dst);
        dst << std::endl;
    }
	virtual void translate(std::ostream &dst) const override{
		
		if (right != NULL){
		//dst << "\treturn ";
        left->translate(dst);
		dst << " + ";
        right->translate(dst);
		}
		else { 
		left->translate(dst);
		dst <<std::endl; }
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeSavedRegisters();
		//dst <<"\tmov\t$" << free[0] << ",$2" << std::endl;
		if (right != NULL){
		//dst << "\treturn ";
	       
		left->print_mips(dst, cont, 2);
		dst << "\taddu\t" <<"$"<< free[5]<<",$"<< free[5] << ",$2"<< std::endl;
		right->print_mips(dst, cont, 2);
		}
		else { 
		left->print_mips(dst, cont, RegisterLock);

			}
		//dst << "\taddu\t" <<"$"<< free[0]<<",$"<< free[0] << ",$2"<< std::endl;
	}
};
	
class MulReturnList : public Ast_Node{
	public:
	NodePtr left;
    NodePtr right;

	
    MulReturnList(NodePtr _left, NodePtr _right) :
        left(_left),
        right(_right) {}

    ~MulReturnList()
    {
        delete left;
        delete right;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst << std::endl;
        left->print(dst);
        dst << " * ";
        right->print(dst);
        dst << std::endl;
    }
	virtual void translate(std::ostream &dst) const override{
		
		if (right != NULL){
		//dst << "\treturn ";
        left->translate(dst);
		dst << " * ";
        right->translate(dst);
		}
		else { 
		left->translate(dst);
		dst <<std::endl; }
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		if (right != NULL){
		//dst << "\treturn ";
        left->print_mips(dst, cont, RegisterLock);
        right->print_mips(dst, cont, RegisterLock);
		}
		else { 
		left->print_mips(dst, cont, RegisterLock);

	}
	}
};

class MinReturnList : public Ast_Node{
	public:
	NodePtr left;
    NodePtr right;

	
    MinReturnList(NodePtr _left, NodePtr _right) :
        left(_left),
        right(_right) {}

    ~MinReturnList()
    {
        delete left;
        delete right;
    }

    virtual void print(std::ostream &dst) const override
    {
        dst << std::endl;
        left->print(dst);
		dst << " - ";
        right->print(dst);
        dst << std::endl;
    }
	virtual void translate(std::ostream &dst) const override{
		
		if (right != NULL){
		//dst << "\treturn ";
        left->translate(dst);
		dst << " - ";
        right->translate(dst);
		}
		else { 
		left->translate(dst);
		dst <<std::endl; }
	}
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		if (right != NULL){
		//dst << "\treturn ";
        left->print_mips(dst, cont, RegisterLock);
        right->print_mips(dst, cont, RegisterLock);
		}
		else { 
		left->print_mips(dst, cont, RegisterLock);

	}
	}
};

class ParameterList : public Parameter{
	public:
	NodePtr left;
    NodePtr right;
	
    ParameterList(NodePtr _left, NodePtr _right) :
        left(_left),
        right(_right) {}

    ~ParameterList()
    {
        delete left;
        delete right;
    }

    virtual void print(std::ostream &dst) const override
    {
        left->print(dst);
        dst << ",";
        right->print(dst);
    }
	virtual void translate(std::ostream &dst) const override{
		left->translate(dst);
        dst << ",";
        right->translate(dst);
	}
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		 left->print_mips(dst, cont, RegisterLock);
         right->print_mips(dst, cont, RegisterLock);
	}
};



#endif