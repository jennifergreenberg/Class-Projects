/* symtab_exception.h
  Team Texas - Jose Paterno, Jessica To, Quoc Lien, Bryant To
  masc1216
	Exception class for opcodetab
	CS530 Spring 2014
	Alan Riggins
*/
#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_EXCEPTION_H

#include <string>

using namespace std;

class symtab_exception {
	
public:
	symtab_exception(string message) {
		this->message = message;
	}

	symtab_exception() {
		message = "An error has occurred.";
	}

	string getMessage(){
		return message;
	}

private:
	string message;
};

#endif
