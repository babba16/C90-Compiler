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
	
	int indexLocal;
	int indexGlobal;
	
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
// Argument Register
	regs[4] = true;
	regs[5] = true;
	regs[6] = true;
	regs[7] = true;
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
//Counting the number of Global variables declared in the program
	globlVar = 0;

//Number of arguments allowed
	argsNo = 4;
	savedNo = 8;
	indexLocal = 4;
	indexGlobal = 4;
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
	indexLocal = c->indexLocal;
	indexGlobal = c->indexGlobal;
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

int getLocal() {
	return indexLocal;
}
	
void setUnused(unsigned int i) {
	regs[i] = false;
}

// for Global Variables
void AddGlobal(const std::string &name) {
	globalBindings.emplace(name, indexGlobal);
	indexGlobal =indexGlobal + 4;
}

void DeleteGlobal(const std::string &name) {
	globalBindings.erase(name);
}

unsigned int FindGlobal(const std::string &name) {
	
	if(globalBindings.find(name) == globalBindings.end()) {
		// not found
		cout << "Global variable " << name << " not found" << endl;
		exit(1);
	} else {
		// found
		return globalBindings.find(name)->second;
	}
}

// for LocalVariables
void AddVariable(const std::string &name) {
	variableBindings.emplace(name, indexLocal);
	indexLocal =indexLocal + 4;
}

void DeleteVariable(const std::string &name) {
	variableBindings.erase(name);
}

unsigned int FindVariable(const std::string &name) {
	
	if(variableBindings.find(name) == variableBindings.end()) {
		// not found
		cout << "Variable " << name << " not found on stack" << endl;
		exit(1);
	} else {
		// found
		return variableBindings.find(name)->second;
	}
	
}

};


#endif

/* Potentially useful for checking if values are actually in the stack
// find where something is and return it's position on stack/registers
bool Context::isOnStack(const std::string *name) {
	
	if(this->isVariable(name)) {
		// it's a variable or a parameter
		return true;
		
	} else if (this->isDynamic(name)) {
		// it's a dynamically-allocated variable
		return true;
		
	} else if (this->isGlobal(name)) {
		// it's a global variable
		return true;
		
	} else {
		// it's not on the stack
		return false;
	}
}

 
std::string findOnStack(const std::string *name) {
	
	if(this->isVariable(name)) {
		// it's a variable or a parameter
		int loc = this->findVariable(name);
		return std::to_string(loc * -4) + "($fp)";
		
	} else if (this->isDynamic(name)) {
		// it's a dynamically-allocated variable
		int dynNo = dynamicBindings.size();
		unsigned int loc = this->findDynamic(name);
		return std::to_string((dynNo - 1 - loc)*8) + "($sp)";
		
	} else if (this->isGlobal(name)) {
		// it's a global variable
		//return *name + "($gp)";
		return "%got(" + *name + ")($28)";
		
	}
	
	return "something went wrong with variable allocation";
}

*/