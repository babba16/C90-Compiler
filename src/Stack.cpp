#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

unordered_map<std::string,int> globalBindings;
int indexLocal = 4;

void AddVariable(const std::string &name);

int main()
{
	
	AddVariable("cheese");
	AddVariable("cheese2");
	AddVariable("cheese3");
	
    std::cout << ((globalBindings.find("cheese")->second)+2)*4 << std::endl;
	
    std::cout << ((globalBindings.find("cheese2")->second)+2)*4 << std::endl;
	
    std::cout << ((globalBindings.find("cheese3")->second)+2)*4 << std::endl;
	return 0;

}

void AddVariable(const std::string &name) {
	int loc = globalBindings.size();
	globalBindings.emplace(name, loc);
}