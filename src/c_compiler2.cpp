#include "../include/ast.hpp"
#include <stdio.h>
#include <string>



int main(int argc, char *argv[])
{
	
	Context cont = new Context();
	
	//std::cout<< "START " << std::endl;	
	//freopen (argv[2],"r",stdin);

    const Ast_Node *ast=parseAST();
	std::cout<< std::endl;	
	
	//freopen (argv[4],"w",stdout);
	//std::string test = argv[1]; 
	
	
	//std::cout<< "The MIPS Equivalent: " << std::endl;
	
	ast->print_mips(std::cout, cont, 2);
    std::cout<<std::endl;
	
	


    return 0;
}
