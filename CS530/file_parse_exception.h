/* file_parse_exception.h
	TEAM TEXAS
	Quoc Lien, Jose Paterno, Jessica To, Bryant To.
	masc1216
	prog1
	CS530, Spring 2014
*/

#ifndef FILE_PARSE_EXCEPTION_H
#define FILE_PARSE_EXCEPTION_H

#include <string>

using namespace std;

class file_parse_exception {
	
public:
	file_parse_exception(string message) {
		this->message = message;
	}

	file_parse_exception() {
		message = "An error has occurred.";
	}

	string getMessage(){
		return message;
	}

private:
	string message;
};

#endif
