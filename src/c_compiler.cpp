#include "../include/ast.hpp"
#include <stdio.h>
#include <string>


int main(int argc, char *argv[])
{
	Context cont = new Context();
	std::cout<< "START " << std::endl;	
	freopen (argv[2],"r",stdin);

    const Ast_Node *ast=parseAST();
	std::cout<< std::endl;	
	
	freopen (argv[4],"w",stdout);
	std::string test = argv[1]; 
	if (test == "--translate") {
	
	//std::cout<< "The Python Equivalent: " << std::endl;
	
	ast->translate(std::cout);
    std::cout<<std::endl;
	}
	if (test == "-S"){

		ast->print_mips(std::cout, cont, 2);
		std::cout<<std::endl;
	}
	else{
		//std::cout<< "The Python Equivalent: " << std::endl;
	}
	
	
	/*else {
		
		
	}*/


    return 0;
}
