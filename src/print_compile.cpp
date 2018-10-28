
#include "ast.hpp"
#include <stdio.h>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	std::cout<< "START " << std::endl;	
	freopen (argv[2],"r",stdin);

    const Ast_Node *ast=parseAST();
	std::cout<< std::endl;	
	
	freopen (argv[4],"w",stdout);
	std::string test = argv[1]; 
	
	//std::cout<< "The Python Equivalent: " << std::endl;
	
	ast->translate(std::cout);
    std::cout<<std::endl;
	/*
	
	//cout << "now printing the MIPS I code" << endl;
	cout << "    .file       1 \"test.c\"" << endl;				// TODO
	cout << "    .section    .mdebug.abi32" << endl;
	cout << "    .previous   " << endl;
	cout << "    .nan        legacy" << endl;
	cout << "    .module     fp=xx" << endl;
	cout << "    .module     nooddspreg" << endl;
	cout << "    .abicalls   " << endl;
	cout << endl;
    ast->compile(ctxt, 99);
    cout << endl;

	/*
    return 0;
}