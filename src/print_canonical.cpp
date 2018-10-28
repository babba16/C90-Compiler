#include "ast.hpp"
#include <stdio.h>
#include <string>


int main(int argc, char *argv[])
{
	Context cont = new Context();
	
	const Ast_Node *ast=parseAST();

	/*std::cout<< "START " << std::endl;	
	freopen (argv[2],"r",stdin);

	std::cout<< std::endl;	

	
	*/
	
	std::cout<< "The Python Equivalent: " << std::endl;
	
	ast->translate(std::cout);
    std::cout<<std::endl;
	
	
	/*
	std::cout<< "The MIPS Equivalent: " << std::endl;
	
	std::cout<< "\t.text" << std::endl;
	ast->print_mips(std::cout, cont, 2);
    std::cout<<std::endl;
	
	
	else {
		
	//std::cout<< "The C Equivalent: " << std::endl;
		ast->print(std::cout);
		std::cout<<std::endl;
	}*/


    return 0;
}
