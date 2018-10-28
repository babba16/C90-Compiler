#ifndef ast_unary_operators_hpp
#define ast_unary_operators_hpp

#include <string>
#include <iostream>
#include <cmath>

extern int scopes;
  
class PostIncrement: public Ast_Node
{
	
public:
    std::string variable;

    PostIncrement(std::string _variable)
        : variable(_variable)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << variable << "++" << std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {

		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << variable << "=" << variable << "+ 1" << std::endl;
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		
		int offset = cont.FindVariable(variable, scopes);
			
		dst <<"\tlw\t" << "$" << RegisterLock << ", " << offset << "($fp)" << std::endl;
		
		dst << "    addiu       $" << RegisterLock << ", $" << RegisterLock << ", 1" << endl;
		
		dst <<"\tsw\t" << "$" << RegisterLock << ", " << offset << "($fp)" << std::endl;
		
		cont.setUnused(free[0]);
		
	}
};

class PostDecrement: public Ast_Node
{
	
public:
    std::string variable;

    PostDecrement(std::string _variable)
        : variable(_variable)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << variable << "--" << std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {

		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << variable << "=" << variable << "- 1" << std::endl;
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		int offset = cont.FindVariable(variable, scopes);
			
		dst <<"\tlw\t" << "$" << RegisterLock << ", " << offset << "($fp)" << std::endl;
		
		dst << "    addiu       $" << RegisterLock << ", $" << RegisterLock << ", -1" << endl;
		
		dst <<"\tsw\t" << "$" << RegisterLock << ", " << offset << "($fp)" << std::endl;
		cont.setUnused(free[0]);
		
		
	}
};

class PreIncrement: public Ast_Node
{
	
public:
    std::string variable;

    PreIncrement(std::string _variable)
        : variable(_variable)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << "++" << variable << std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {

		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << variable << "=" << variable << "+ 1" << std::endl;
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		dst << "    addiu       $" << free[0] << ", $" << free[0] << ", 1" << endl;
		cont.setUnused(free[0]);
		
	}
};

class PreDecrement: public Ast_Node
{
	
public:
    std::string variable;

    PreDecrement(std::string _variable)
        : variable(_variable)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << "--" << variable <<  std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {

		for (int i =0; i < scopes; i++){dst << "\t";}
		dst << variable << "=" << variable << "- 1" << std::endl;
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeSavedRegisters();
		cont.setUsed(free[0]);
		dst << "    addiu       $" << free[0] << ", $" << free[0] << ", -1" << endl;
		cont.setUnused(free[0]);
		
		
	}
};

class AddAssign: public Ast_Node
{
	
public:
    std::string variable;
	ExpressionPtr right;

    AddAssign(std::string _variable, ExpressionPtr _right)
        : variable(_variable), right(_right)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << "+=" << variable <<  std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {
// NOt implemented
		
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeTmpRegisters();
		cont.setUsed(free[0]);
		cont.setUsed(free[1]);
		right->print_mips(dst, cont, free[0]);
		
		dst <<"\tlw\t" << "$" << free[1] << ", " << cont.FindVariable(variable, scopes) << "($fp)" << std::endl;
		
		dst << "\taddu\t $" << free[0] << ", $" << free[0] << ",$" << free[1] << endl;
		
		dst << "\tsw\t$" << free[0]<<  ","<< cont.FindVariable(variable, scopes) <<"($fp)" << std::endl;
		cont.setUnused(free[0]);
		cont.setUnused(free[1]);
		
		
	}
};

class SubAssign: public Ast_Node
{
	
public:
    std::string variable;
	ExpressionPtr right;

    SubAssign(std::string _variable, ExpressionPtr _right)
        : variable(_variable), right(_right)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << "-=" << variable <<  std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {
// NOt implemented
		
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeTmpRegisters();
		cont.setUsed(free[0]);
		cont.setUsed(free[1]);
		right->print_mips(dst, cont, free[0]);
		
		dst <<"\tlw\t" << "$" << free[1] << ", " << cont.FindVariable(variable, scopes) << "($fp)" << std::endl;
		
		dst << "\tsub\t$" << free[0] << ", $" << free[1] << ",$" << free[0] << endl;
		
		dst << "\tsw\t$" << free[0]<<  ","<< cont.FindVariable(variable, scopes) <<"($fp)" << std::endl;
		cont.setUnused(free[0]);
		cont.setUnused(free[1]);
		
		
	}
};

class MulAssign: public Ast_Node
{
	
public:
    std::string variable;
	ExpressionPtr right;

    MulAssign(std::string _variable, ExpressionPtr _right)
        : variable(_variable), right(_right)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << "*=" << variable <<  std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {
// NOt implemented
		
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeTmpRegisters();
		cont.setUsed(free[0]);
		cont.setUsed(free[1]);
		right->print_mips(dst, cont, free[0]);
		
		dst <<"\tlw\t" << "$" << free[1] << ", " << cont.FindVariable(variable, scopes) << "($fp)" << std::endl;
		
		dst << "\tmult\t" <<"$"<< free[0]<<", $"<< free[1]<< std::endl;
		dst << "\tmflo\t" <<"$"<< free[0]<< std::endl;
		
		dst << "\tsw\t$" << free[0]<<  ","<< cont.FindVariable(variable, scopes) <<"($fp)" << std::endl;
		cont.setUnused(free[0]);
		cont.setUnused(free[1]);
		
		
	}
};

class DivAssign: public Ast_Node
{
	
public:
    std::string variable;
	ExpressionPtr right;

    DivAssign(std::string _variable, ExpressionPtr _right)
        : variable(_variable), right(_right)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << "/=" << variable <<  std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {
// NOt implemented
		
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeTmpRegisters();
		cont.setUsed(free[0]);
		cont.setUsed(free[1]);
		right->print_mips(dst, cont, free[0]);
		
		dst <<"\tlw\t" << "$" << free[1] << ", " << cont.FindVariable(variable, scopes) << "($fp)" << std::endl;
		
		dst << "\tdiv\t" <<"$"<< free[1]<<", $"<< free[0]<< std::endl;
		dst << "\tmflo\t" <<"$"<< free[1]<< std::endl;
		
		dst << "\tsw\t$" << free[1]<<  ","<< cont.FindVariable(variable, scopes) <<"($fp)" << std::endl;
		cont.setUnused(free[0]);
		cont.setUnused(free[1]);
		
		
	}
};

class RemAssign: public Ast_Node
{
	
public:
    std::string variable;
	ExpressionPtr right;

    RemAssign(std::string _variable, ExpressionPtr _right)
        : variable(_variable), right(_right)
    {}
	

 virtual void print(std::ostream &dst) const override
    {
        dst << "/=" << variable <<  std::endl;
    }
	virtual void translate(std::ostream &dst) const override
    {
// NOt implemented
		
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		vector<int> free = cont.freeTmpRegisters();
		cont.setUsed(free[0]);
		cont.setUsed(free[1]);
		right->print_mips(dst, cont, free[0]);
		
		dst <<"\tlw\t" << "$" << free[1] << ", " << cont.FindVariable(variable, scopes) << "($fp)" << std::endl;
		
		dst << "\tdiv\t" <<"$"<< free[1]<<", $"<< free[0]<< std::endl;
		dst << "\tmfhi\t" <<"$"<< free[1]<< std::endl;
		
		dst << "\tsw\t$" << free[1]<<  ","<< cont.FindVariable(variable, scopes) <<"($fp)" << std::endl;
		cont.setUnused(free[0]);
		cont.setUnused(free[1]);
		
		
	}
};

#endif
 