#ifndef ast_program_hpp
#define ast_program_hpp

#include <string>
#include <iostream>

class ProgramList : public Ast_Node
{
public:
	public:
	NodePtr SingleProgram;
    NodePtr ListofProgram;
	
    ProgramList(NodePtr _SingleProgram, NodePtr _ListofProgram) :
        SingleProgram(_SingleProgram),
        ListofProgram(_ListofProgram) {}

    ~ProgramList()
    {
        delete SingleProgram;
        delete ListofProgram;
    }
	
	virtual void translate(std::ostream &dst) const override{
		SingleProgram->translate(dst);
        dst << std::endl;
        ListofProgram->translate(dst);
	}

    virtual void print(std::ostream &dst) const override
    {
        SingleProgram->print(dst);
        dst << std::endl;
        ListofProgram->print(dst);
    }
   
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		SingleProgram->print_mips(dst, cont, RegisterLock);
		ListofProgram->print_mips(dst, cont, RegisterLock);
	}

};

class Program : public Ast_Node
{
public:
	public:
	NodePtr SingleProgram;
 
	
    Program(NodePtr _SingleProgram) :
        SingleProgram(_SingleProgram) {}

    ~Program()
    {
        delete SingleProgram;

    }
	
	virtual void translate(std::ostream &dst) const override{
		SingleProgram->translate(dst);
        dst << std::endl;
	}

    virtual void print(std::ostream &dst) const override
    {
        SingleProgram->print(dst);
        dst << std::endl;
    }
   
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const override {
		SingleProgram->print_mips(dst, cont, RegisterLock);
	}

};


#endif
