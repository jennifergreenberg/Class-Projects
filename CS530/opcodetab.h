/* opcodetab.h
 * TEAM TEXAS
 * Quoc Lien, Jose Paterno, Jessica To, Bryant To.
 * masc1216
 * prog2
 * CS530, Spring 2014
*/

#ifndef OPCODETAB_H_
#define OPCODETAB_H_

#include <ctime>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <iostream>
#include "opcode_error_exception.h"

using namespace std;

const string CODES [] = { "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF",
                          "DIVR", "FIX", "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", 
                          "LDB", "LDCH", "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", "MUL", "MULF", 
                          "MULR", "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR", "SIO", "SSK", 
                          "STA", "STB", "STCH", "STF", "STI", "STL", "STS", "STSW", "STT", "STX", 
                          "SUB", "SUBF", "SUBR", "SVC", "TD", "TIO", "TIX", "TIXR", "WD"};
			
const string MCODE [] = {"18","58","90","40","B4","28","88","A0","24","64",
                         "9C","C4","C0","F4","3C","30","34","38","48","00",
                         "68","50","70","08","6C","74","04","D0","20","60",
                         "98","C8","44","D8","AC","4C","A4","A8","F0","EC",
                         "0C","78","54","80","D4","14","7C","E8","84","10",
                         "1C","5C","94","B0","E0","F8","2C","B8","DC"};
	
const int INSTR_SIZE [] = {3,3,2,3,2,3,3,2,3,3,
                           2,1,1,1,3,3,3,3,3,3,
                           3,3,3,3,3,3,3,3,3,3,
                           2,1,3,3,2,3,2,2,1,3,
                           3,3,3,3,3,3,3,3,3,3,
                           3,3,2,2,3,1,3,2,3};

const string LULZ_ERRS [] = {"Opcode not found. Not our problem, Mr. Riggins. It's all you!", 
                                "Opcode not found. Rotten Bananas!", 
                                "Opcode not found. We're not good enough for you....", 
                                "Opcode not found. Please don't deduct points from us ):",
                                "Opcode not found. Please stop trying to break our code, it's not funny if you do", 
                                "Opcode not found. Nice try, but no. No cookie for you Mr. Riggins!"}; 


class opcodetab {
    public:
        // ctor
        // creates a new dictionary structure and loads all of the opcodes for
        // the SIC/XE architecture into the table.  Use the STL map for this.
        opcodetab();

        // takes a SIC/XE opcode and returns the machine code
        // equivalent as a two byte string in hexadecimal.
        // Example:  get_machine_code("ADD") returns the value 18
        // Note that opcodes may be prepended with a '+'.
        // throws an opcode_error_exception if the opcode is not
        // found in the table.
        string get_machine_code(string opcode);

        // takes a SIC/XE opcode and returns the number of bytes
        // needed to encode the instruction, which is an int in
        // the range 1..4.
        // NOTE: the opcode must be prepended with a '+' for format 4.
        // throws an opcode_error_exception if the opcode is not
        // found in the table.
        int get_instruction_size(string opcode);

    private:
        map<string,pair<string,int> > m;
        map<string, pair<string,int> >::iterator m_iter;

        string get_code(string opcode);		
    	bool opcode_exists (string s);
        string upper(string s);
        bool ext_valid(string s);
};

#endif
