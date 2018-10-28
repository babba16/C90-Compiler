#ifndef ast_operators_hpp
#define ast_operators_hpp

#include <string>
#include <iostream>
#include <cmath>

extern int LoopsNums;

class Operator
    : public Expression
{
protected:
    ExpressionPtr left;
    ExpressionPtr right;

    Operator(ExpressionPtr _left, ExpressionPtr _right)
        : left(_left)
        , right(_right)
    {}
	
public:
    virtual const char *getOpcode() const =0;
	

    ExpressionPtr getLeft() const
    { return left; }

    ExpressionPtr getRight() const
    { return right; }

    virtual void print(std::ostream &dst) const override
    {
        left->print(dst);
        dst<< " " << getOpcode() << " ";
        right->print(dst);
    }
	virtual void translate(std::ostream &dst) const override
    {

        left->translate(dst);
		if ( getOpcode() == "and" || getOpcode() == "or"){
				dst<< " " << getOpcode() << " ";
		} 
		else {
       dst<< getOpcode();
		}
	   if (right!= NULL){
        right->translate(dst);
	   }
    }
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		
		vector<int> free = cont.freeTmpRegisters();
		cont.setUsed(free[0]);
		std::string check = getOpcode();

		if (check == "+"){
			
		
		left->print_mips(dst, cont, RegisterLock);
	
		
		right->print_mips(dst, cont, free[1]);
		
		dst << "\taddu\t" <<"$"<< RegisterLock<<",$"<< RegisterLock << ",$"<< free[1]<< std::endl;
		
		cont.setUnused(free[0]);
		
		}
		else if (check == "-"){
		
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tsub\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ",$"<< free[1]<< std::endl;
		
		cont.setUnused(free[0]);
		}
		else if (check == "!"){
		
		//left->print_mips(dst, cont, RegisterLock);
		
		right->print_mips(dst, cont, free[1]);
		cont.setUsed(free[2]);
		dst << "\taddi\t$" << free[2] << ", $0, -1" << endl;
		dst << "\txor\t$" << free[1] << ", $" << free[1] << ", $" << free[2] << endl;
		
		cont.setUnused(free[2]);
		cont.setUnused(free[0]);
		}
		else if (check == "~"){
		
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tnor\t" <<"$"<< RegisterLock<<",$"<< free[1] << ",$"<< free[1]<< std::endl;
		}
		else if (check == "^"){
		
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\txor\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ",$"<< free[1]<< std::endl;
		
		cont.setUnused(free[0]);
		}
		else if (check == "<<"){
		
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tsllv\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ",$"<< free[1]<< std::endl;
		
		cont.setUnused(free[0]);
		}
		
		else if (check == ">>"){
		
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tsrlv\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ",$"<< free[1]<< std::endl;
		
		cont.setUnused(free[0]);
		}
		
		else if (check == "|"){
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tor\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ",$"<< free[1]<< std::endl;		
		cont.setUnused(free[0]);
		}
		
		else if (check == "&"){

		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tand\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ",$"<< free[1]<< std::endl;	
		cont.setUnused(free[0]);
		}
		
		else if (check == "*"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tmult\t" <<"$"<< RegisterLock<<", $"<< free[1]<< std::endl;
		dst << "\tmflo\t" <<"$"<< RegisterLock<< std::endl;
		cont.setUnused(free[0]);
		}
		
		else if (check == "/"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tdiv\t" <<"$"<< RegisterLock<<", $"<< free[1]<< std::endl;
		dst << "\tmflo\t" <<"$"<< RegisterLock<< std::endl;
		cont.setUnused(free[0]);
		}
		
		else if (check == "%"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tdiv\t" <<"$"<< RegisterLock<<", $"<< free[1]<< std::endl;
		dst << "\tmfhi\t" <<"$"<< RegisterLock<< std::endl;
		cont.setUnused(free[0]);
		}
		
		else if (check == "<"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tslt\t" <<"$"<< RegisterLock<<",$"<< RegisterLock  << ",$"<< free[1]<< std::endl;
		
		cont.setUnused(free[0]);
		}
		
		else if (check == "<="){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tslt\t" <<"$"<< RegisterLock <<",$"<< free[1] << ",$" << RegisterLock << std::endl;
		dst << "\txori\t" <<"$"<< RegisterLock << ",$" << RegisterLock << ",0x1" << std::endl; 
		dst << "\tandi\t" <<"$"<< RegisterLock << ",$" << RegisterLock << ",0x00ff" << std::endl;
		//dst << "\tbeq\t" << "$"<< RegisterLock<<",$0"<< ", $end" << LoopsNums << std::endl;
		cont.setUnused(free[0]);
		}
		
		else if (check == ">="){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tslt\t" <<"$"<< RegisterLock <<",$"<< RegisterLock << ",$" << free[1] << std::endl;
		dst << "\txori\t" <<"$"<< RegisterLock << ",$" << RegisterLock << ",0x1" << std::endl; 
		dst << "\tandi\t" <<"$"<< RegisterLock << ",$" << RegisterLock << ",0x00ff" << std::endl;
		//dst << "\tbeq\t" << "$"<< RegisterLock<<",$0"<< ", $end" << LoopsNums << std::endl;
		cont.setUnused(free[0]);
		}
		
		else if (check == ">"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tslt\t" <<"$"<< RegisterLock<<",$"<< free[1] << ",$"<< RegisterLock<< std::endl;
		
		cont.setUnused(free[0]);
		}
		
		else if (check == "/"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tdiv\t" <<"$"<< RegisterLock<<", $"<< free[1]<< std::endl;
		dst << "\tmflo\t" <<"$"<< RegisterLock<< std::endl;
		cont.setUnused(free[0]);
		}
		
		else if (check == "and"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tand\t" <<"$"<< RegisterLock<< ", $" << RegisterLock<<", $"<< free[1]<< std::endl;

		cont.setUnused(free[0]);
		}
		
		else if (check == "or"){
			
		left->print_mips(dst, cont, RegisterLock);
	
		right->print_mips(dst, cont, free[1]);
		
		dst << "\tor\t" <<"$"<< RegisterLock<< ", $" << RegisterLock<<", $"<< free[1]<< std::endl;

		cont.setUnused(free[0]);
		}
		
		else if (check == "=="){
			
		left->print_mips(dst, cont, RegisterLock);
		right->print_mips(dst, cont, free[1]);
		
		//Test for the == condition
		dst << "\tbne\t" <<"$"<< RegisterLock<< ", $"<< free[1]<<", $not" << LoopsNums << std::endl;
		dst<<  "\tnop\t" << std::endl;
		
		dst <<  "\tli\t" << "$" << RegisterLock << ", 1" << std::endl;
		dst <<  "\tb\t" << "$endl" << LoopsNums << std::endl;	
		dst <<  "\tnop\t" << std::endl;

		dst << "$not" << LoopsNums << ":" << std::endl;
		dst <<  "\tmove\t" << "$" << RegisterLock << ", $0" << std::endl;
		
		
		dst << "$endl" << LoopsNums << ":" << std::endl;	
		
		cont.setUnused(free[0]);
		}
		
		else if (check == "!="){
			
		left->print_mips(dst, cont, RegisterLock);
		right->print_mips(dst, cont, free[1]);
		
		//Test for the != condition
		dst << "\tbeq\t" <<"$"<< RegisterLock<< ", $"<< free[1]<<", $not" << LoopsNums << std::endl;
		dst<<  "\tnop\t" << std::endl;
		
		dst <<  "\tli\t" << "$" << RegisterLock << ", 1" << std::endl;
		dst <<  "\tb\t" << "$endl" << LoopsNums << std::endl;	
		dst <<  "\tnop\t" << std::endl;

		dst << "$not" << LoopsNums << ":" << std::endl;
		dst <<  "\tmove\t" << "$" << RegisterLock << ", $0" << std::endl;
		
		
		dst << "$endl" << LoopsNums << ":" << std::endl;	
				
		cont.setUnused(free[0]);
		}
		else {
			dst <<  "\tnop\t" << std::endl;
			cont.setUnused(free[0]);
		}

	}
};

class AddOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "+"; }
	
public:
    AddOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}
    

};

class SubOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "-"; }
public:
    SubOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}
	
    
};


class MulOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "*"; }
public:
    MulOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class RemOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "%"; }
public:
    RemOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};
class DivOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "/"; }
public:
    DivOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};


class LessThanOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "<"; }
public:
   LessThanOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};


class LessThanEqualOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "<="; }
public:
    LessThanEqualOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}


};

class MoreThanEqualOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return ">="; }
public:
    MoreThanEqualOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class MoreThanOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return ">"; }
public:
    MoreThanOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};


class EqualToOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "=="; }
public:
    EqualToOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class NotEqualToOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "!="; }
public:
    NotEqualToOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};


class LogicalAndOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "and"; }
public:
    LogicalAndOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class LogicalOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "or"; }
public:
    LogicalOrOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class BitwiseAndOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "&"; }
public:
    BitwiseAndOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class BitwiseXorOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "^"; }
public:
    BitwiseXorOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class BitwiseOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "|"; }
public:
    BitwiseOrOperator(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class LeftShift
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "<<"; }
public:
    LeftShift(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class ExclamationNot
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "!"; }
public:
    ExclamationNot(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class LogicalNot
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "~"; }
public:
    LogicalNot(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};

class RightShift
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return ">>"; }
public:
    RightShift(ExpressionPtr _left, ExpressionPtr _right)
        : Operator(_left, _right)
    {}

};



#endif
