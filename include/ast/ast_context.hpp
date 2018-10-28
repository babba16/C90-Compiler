#ifndef context_hpp
#define context_hpp

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>

using namespace std;


class Context {
public:
	bool regs[32];
	//Counting the number of arguments	
	int paramNo;
//Counting the number of Local variables declared in our scope
	int varNo;
//Counting the number of Global variables declared in the origran
	int globlVar;
	
//Number of arguments allowed
	int argsNo;
	int savedNo;
	
	
	//Local Variables
	unordered_map<std::string, unsigned int> variableBindings;		
	//Global Variables
	unordered_map<std::string, unsigned int> globalBindings;	

Context() {
	for(int i = 0; i < 32; i++) {
		regs[i] = false;
	}
	regs[0] = true; // Zero Register
	regs[1] = true; 
	
// Return Registers
	
	regs[2] = true; 
	regs[3] = true;

// Can't be used - Kernel Reserved Register
	regs[26] = true;
	regs[27] = true;
//Global Pointer
	regs[28] = true;
//Stack Pointer
	regs[29] = true;
//Frame Pointer
	regs[30] = true;
//Return Address in a subroutine
	regs[31] = true;
	

//Counting the number of arguments	
	paramNo = 0;
//Counting the number of Local variables declared in our scope
	varNo = 0;
//Counting the number of Global variables declared in the origran
	globlVar = 0;

//Number of arguments allowed
	argsNo = 4;
	savedNo = 8;
	
}
	
Context(Context* c) {
	for(int i = 0; i < 32; i++) {
		regs[i] = c->regs[i];
	}
	//SP = c->SP;
	//fsize = c->fsize;
	//fsize_org = c->fsize_org;
	paramNo = c->paramNo;
	varNo = c->varNo;
	globlVar = c->globlVar;
	argsNo = c->argsNo;
	savedNo = c->savedNo;
}

~Context() {}

//Checks which of the saved registers are free
vector<int> freeSavedRegisters() {
	
	vector<int> free;
	
	for(int i = 16; i < 24; i++) {
		if(!regs[i]) {
			free.push_back(i);
		}
	}
	
	if(free.empty()) {
		cout << "no free registers" << endl;
		exit(1);
	}
	
	return free;
	
}

vector<int> freeArgumentRegisters() {
	
	vector<int> free;
	
	for(int i = 4; i < 7; i++) {
		if(!regs[i]) {
			free.push_back(i);
		}
	}
	
	if(free.empty()) {
		cout << "no free registers" << endl;
		exit(1);
	}
	
	return free;
	
}
//Checks which of the temporary registers are free
vector<int> freeTmpRegisters() {
	
	vector<int> free;
	
	for(int i = 8; i < 16; i++) {
		if(!regs[i]) {
			free.push_back(i);
		}
	}
	
	for(int i = 24; i < 26; i++) {
		if(!regs[i]) {
			free.push_back(i);
		}
	}
	
	if(free.empty()) {
		cout << "no free temporary registers" << endl;
		exit(1);
	}
	
	return free;
	
}


// Checking 
void setUsed(unsigned int i) {
	regs[i] = true;
}
	
void setUnused(unsigned int i) {
	regs[i] = false;
}

// for Global Variables
void AddGlobal(const std::string name) {
	int loc = globalBindings.size();
	globalBindings.emplace(name, loc);
}


void DeleteGlobal(const std::string name) {
	globalBindings.erase(name);
}

unsigned int FindGlobal(const std::string name) {
	
	if(globalBindings.find(name) == globalBindings.end()) {
		// not found
		return 0;
	} else {
		// found
		return 1;
	}
}

// for LocalVariables
void AddVariable(const std::string name) {
	unsigned int loc = variableBindings.size();
	variableBindings.emplace(name, loc);
}

void DeleteVariable(const std::string name) {
	variableBindings.erase(name);
}

int FindVariable(const std::string name, int scope) {
	
	std::string test = name + std::to_string(scope);

	while (scope > -1){
		std::string test = name + std::to_string(scope);	
	if(variableBindings.find(test) == variableBindings.end()) {
		scope--;
			}
	if(scope == -1){
		cout << "Variable " << name << " not found on stack" << endl;
		exit(1);
		//return 0;
	}
	else {
		// found
		return ((variableBindings.find(test)->second)+2)*4;
	}
	}
	
}

};


#endif