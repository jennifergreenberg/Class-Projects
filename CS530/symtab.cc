/* symtab.cc
 * TEAM TEXAS
 * Quoc Lien, Jose Paterno, Jessica To, Bryant To.
 * masc1216
 * prog3
 * CS530, Spring 2014
*/

 #include "symtab.h"

 void symtab::add(string symbol, string value, bool type) {
 	if(!exists(symbol)) {
 		m[symbol] = pair<string,bool>(value, type);
 	}
 	else {
 		throw symtab_exception("Symbol already defined in map.");
 	}
 }

 void symtab::modify(string symbol, string value, bool type) {
 	if(exists(symbol)) {

 		m_iter =  m.find(symbol);
 		m_iter->second.first = int_to_hex(string_to_int(value));
 	}
 	else {
 		throw symtab_exception("Cannot modify non-existing symbol.");
 	}
 }
 
 void symtab::add_equ(string symbol, string symbol2){
        m_equ[symbol] = symbol2;
}

string symtab::get_equ(string label){
    if (m_equ.find(label) != m_equ.end())
        return get_equ(m_equ.find(label)->second);
    return label;    
}

void symtab::assign_equ(string symbol){
    cout << get_equ(m_equ.find(symbol)->second) << endl;
    string tmp_value = get_equ(m_equ.find(symbol)->second);
    for(equ_iter = m_equ.begin(); equ_iter != m_equ.end(); ++equ_iter){
        cout << get_equ(symbol) << endl;
        equ_iter->second = tmp_value;
    }
}

void symtab::print_equ(){
    for(equ_iter = m_equ.begin(); equ_iter != m_equ.end(); ++equ_iter) {
 		cout << equ_iter->first << "\t" << equ_iter->second << endl;
 	}   
}

 string symtab::get_value(string symbol) {
 	if(symbol.find(',')) {
 		stringstream str(symbol);
 		getline(str,symbol,',');
 	}
 	if(symbol[0] == '#' || symbol[0] == '@') {
 		symbol.erase(0,1);

 		if(exists(symbol)) {
 			m_iter =  m.find(symbol);

 			return m_iter->second.first;
 		}
 		else {
 			return to_uppercase(int_to_hex(string_to_int(symbol)));
 		}
 	}

 	if(exists(symbol)) {
 		m_iter =  m.find(symbol);
 		return m_iter->second.first;
 	}
 	else {
 		throw symtab_exception("Symbol not found.");
 	}
 }

 void symtab::print_table() {
 	for(m_iter = m.begin(); m_iter != m.end(); ++m_iter) {
 		cout << m_iter->first << "\t" << m_iter->second.first << endl;
 	}
 }

 bool symtab::exists(string symbol) {
 	if(m.find(symbol) == m.end()) {
 		return false;
 	}
 	return true;
 }

 string symtab::int_to_hex(int num){
	stringstream out;
	out<<setw(5)<<setfill('0')<<hex<<num;
	return out.str();
}

int symtab::string_to_int(string s){
 istringstream instr(s);
 int n;
 instr >> n;
 return n;
}

string symtab::to_uppercase(string s){
	string tmp = s;
	string::iterator iter;

	for(iter = tmp.begin(); iter != tmp.end(); ++iter) {
  		*iter = std::toupper((unsigned char)*iter);
  	}
	return tmp;
}

