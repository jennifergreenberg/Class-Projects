/* file_parser.h
	TEAM TEXAS
	Quoc Lien, Jose Paterno, Jessica To, Bryant To.
	masc1216
	prog1
	CS530, Spring 2014
*/

#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "file_parse_exception.h"

using namespace std;

class file_parser {

public:
	// Constructor, filename is the parameter.  A driver program will read
	// the filename from the command line, and pass the filename to
	// the file_parser constructor.  Filenames must not be hard-coded.
	file_parser(string s);

	// Destructor
	~file_parser(void);

	// reads the source file, storing the information is some
	// auxiliary data structure you define in the private section. 
	// Throws a file_parse_exception if an error occurs.
	// if the source code file fails to conform to the above
	// specification, this is an error condition.     
	void read_file();

	// returns the token found at (row, column).  Rows and columns
	// are zero based.  Returns the empty string "" if there is no 
	// token at that location. column refers to the four fields
	// identified above.
	string get_token(unsigned int line_row, unsigned int label_col);

	// prints the source code file to stdout.  Should not repeat 
	// the exact formatting of the original, but uses tabs to align
	// similar tokens in a column. The fields should match the 
	// order of token fields given above (label/opcode/operands/comments)
	void print_file();

	// returns the number of lines in the source code file
	int size();

private:
	string line;
	string file_name;
	string token;

	unsigned int num;

	struct parse_data{
		string label;
		string opcode;
		string operand;
		string comments;
	};

	parse_data data;

	vector<parse_data> v_data;

	// Resets parse_data structure.
	void reset_data();

	string trim_spaces(string str);
};

#endif
