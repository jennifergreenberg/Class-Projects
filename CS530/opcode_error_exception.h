/* opcode_error_exception.h
  Team Texas - Jose Paterno, Jessica To, Quoc Lien, Bryant To
  masc1216
	Exception class for opcodetab
	CS530 Spring 2014
	Alan Riggins
*/
#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H

#include <string>

using namespace std;

class opcode_error_exception {
	
public:
	opcode_error_exception(string message) {
		this->message = message;
	}

	opcode_error_exception() {
		message = "An error has occurred.";
	}

	string getMessage(){
		return message;
	}

private:
	string message;
};

#endif
