#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>
#include <memory>

#include "ast_context2.hpp"

class Ast_Node;

typedef const Ast_Node *NodePtr;

static int scopes = 0;

static std::vector<std::string> functioncalltest;

static int LoopsNums = 0;

static bool ReturnTest = false;

static int NoVariables = 0;
static bool main2 = false;

static std::vector<std::string> globals;

class Ast_Node
{
public:
    virtual ~Ast_Node()
    {}

	//! Tell and expression to print itself to the given stream
    virtual void print(std::ostream &dst) const =0;
	
	virtual void translate(std::ostream &dst) const =0;
	
	// NEED TO PASS A REGISTER FOR THE ARITHMETIC 
	virtual void print_mips(std::ostream &dst, Context &cont, int RegisterLock) const =0;
	
	
};


#endif
