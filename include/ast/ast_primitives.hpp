#ifndef ast_primitives_hpp
#define ast_primitives_hpp

#include <string>
#include <iostream>

extern int scopes;


class Variable
    : public Expression
{
private:
    std::string id;
public:
    Variable(const std::string &_id)
        : id(_id)
    {}

    const std::string getId() const
    { return id; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<id;
    }
	
	virtual void translate(std::ostream &dst) const override{
		dst<<id;
	}

	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		int offset = cont.FindVariable(id, scopes);
		
		if (offset == 0){
		dst <<"\tlui\t" << "$" << RegisterLock << ",%hi("<< id << ")" << std::endl;
		dst <<"\tlw\t" << "$" << RegisterLock << ",%lo("<< id << ")($" << RegisterLock << ")" << std::endl;
		} 
		else{
		dst <<"\tlw\t" << "$" << RegisterLock << ", " << offset << "($fp)" << std::endl;
		}
	}
};

class Number
    : public Expression
{
private:
    int value;
public:
    Number(int _value)
        : value(_value)
    {}

    double getValue() const
    { return value; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<value;
    }

	virtual void translate(std::ostream &dst) const override{
		dst<<value;
	}
	
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		dst <<"\tli\t" << "$" << RegisterLock << ", " << value << std::endl;
	}
  
};

class Float
    : public Expression
{
private:
    double value;
public:
    Float(double _value)
        : value(_value)
    {}

    double getValue() const
    { return value; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<value;
    }
	
	virtual void translate(std::ostream &dst) const override{
		dst<<value;
	}

	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		dst<<value;
	}
};


#endif
