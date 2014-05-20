/* opcodetab.cc
 * TEAM TEXAS
 * Quoc Lien, Jose Paterno, Jessica To, Bryant To.
 * masc1216
 * prog2
 * CS530, Spring 2014
*/

#include "opcodetab.h"

opcodetab::opcodetab(){
	
	for(unsigned int i = 0;i<59;i++){
		m[CODES[i]]=pair<string,int>(MCODE[i],INSTR_SIZE[i]);
	}	
}

string opcodetab::get_machine_code(string opcode){
	string tmp_opcode = upper(opcode);
	
    if(opcode_exists(get_code(tmp_opcode))){
        tmp_opcode = get_code(tmp_opcode);
        if (opcode[0] == '+'){
            if (ext_valid(tmp_opcode)){
                m_iter = m.find(tmp_opcode);
                return m_iter->second.first;
            }
            else {
                srand( time(0) + rand() );
                throw opcode_error_exception(LULZ_ERRS[rand()%6]);
            }    
        }
        else {
            m_iter = m.find(tmp_opcode);
            return m_iter->second.first;
        }
    }
    else {
        srand( time(0) + rand() );
        throw opcode_error_exception(LULZ_ERRS[rand()%6]);
    }
}

int opcodetab::get_instruction_size(string opcode){
	string tmp_opcode = upper(opcode);
	if(opcode_exists(get_code(tmp_opcode))){
        if (tmp_opcode[0] == '+'){
            if (ext_valid(tmp_opcode))
                return 4;  
            else {
                srand( time(0)+rand() );
		        throw opcode_error_exception(LULZ_ERRS[rand()%6]);
            }
        }     
		else {
			m_iter = m.find(tmp_opcode);
			return m_iter->second.second;
		}
	}
	else {
        	srand( time(0)+rand() );
		throw opcode_error_exception(LULZ_ERRS[rand()%6]);
	}
}

bool opcodetab::opcode_exists(string s) {
	if(m.find(upper(s)) == m.end()) {
		return false;
	}
	return true;
}

string opcodetab::get_code(string opcode) {
	if(opcode[0] == '+') {
		string tmp;

		tmp = opcode.erase(0,1);

		return tmp;
	}
	return opcode;
}

string opcodetab::upper(string s){
	string tmp = s;
	string::iterator i = tmp.begin();
	string::iterator end = tmp.end();

	while (i != end) {
  		*i = std::toupper((unsigned char)*i);
  		++i;
  	}

  	return tmp;
}

bool opcodetab:: ext_valid(string s){
    s = get_code(s);
    m_iter = m.find(s);
    if( m_iter->second.second == 3) 
        return true;   
    return false;
}
