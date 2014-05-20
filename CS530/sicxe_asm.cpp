/* sicxe_asm.cpp
 * TEAM TEXAS
 * Quoc Lien, Jose Paterno, Jessica To, Bryant To.
 * masc1216
 * prog3
 * CS530, Spring 2014
*/

 #include "sicxe_asm.h"

sicxe_asm::sicxe_asm(string filename) {
	asm_address = 0x00000000;
	base = -1;
	start = -1;

	init();

	first(filename);
	second();
	write_file(filename);
	write_object_file(filename);
}

void sicxe_asm::print_equ() {
 	for(equ_iter = equ.begin(); equ_iter != equ.end(); ++equ_iter) {
 		cout << equ_iter->first << "\t" << equ_iter->second << endl;
 	}
 }

void sicxe_asm::assign_address(file_parser parser) {
	for(int i = 1; i < parser.size(); i++) {
		parse_to_struct(parser,i);

		if(check_asm_dir(data.opcode)) {
			if(!(upper(data.opcode) == "END")) {
				handle_asm_dir(data.opcode, data.operand, i);
			}
			else {
				v_data.push_back(data);
				break;
			}
		}
		else {
			if(data.operand[1] == '#'){
				ss << "Error on line: " << i << ". Invalid operand.";
                throw symtab_exception(ss.str());
            }
			if(!ignore_case(data.opcode) && start != -1) {
				try {
					asm_address += opcode.get_instruction_size(data.opcode);
				}
				catch(opcode_error_exception ox) {
					cerr << "An Opcode error has occured at line " << i  << ": " << ox.getMessage() << endl;
				}
			}
			else if(!ignore_case(data.opcode) && start == -1) {
				ss << "Error on line: " << i << ". Invalid operation! Opcode operation before Start.";
				throw opcode_error_exception(ss.str());
			}
		}
		// Adding user defined labels to symbol tabel.
		add_symtab(data.address, data.label, data.operand);

		v_data.push_back(data);
	}
}

void sicxe_asm::handle_asm_dir(string op, string operand, int index) {
	int value;
	string tmp_opcode = upper(op);
	if(operand[0] == '@' || operand[0] == '#' || operand[1] == '@' || operand[1] == '#') {
        	ss << "Error on line: " << index << ". Addressing modes do not apply to assembly directives.";
        	throw symtab_exception(ss.str());
    	}
    	else{
		if(tmp_opcode == "START") {
			start = 1;
			value = int_value(operand);
			asm_address = value;
		}
		else if(tmp_opcode == "BASE") {
			base_operand = operand;
			base = 1;
		}
		else if(tmp_opcode == "NOBASE") {
			base = -1;
		}
		else {
			start = 1;
			value = hex_value(operand);

			int size = asm_dir.find(tmp_opcode)->second; // Getting Operation Size.
	
			if(tmp_opcode == "RESW" || tmp_opcode == "RESB") {
				asm_address += (size * value);
			}
			else if( tmp_opcode == "BYTE") {
				int length = operand.length() - 3;
	
				if(operand[0] == 'x' || operand[0] == 'X') {
					if(length % 2 == 0) {
						asm_address += ((length/2) * size);
					}
					else {
						ss << "Error on line: " << index << ". Invalid operand size. Hex value should be even.";
						throw symtab_exception(ss.str());
					}
				}
				else if(operand[0] == 'c' || operand[0] == 'C') {
					asm_address += (length * size);
				}
				else {
					asm_address += size;
				}
			}
			else {
				asm_address += size;
			}
		}
    }
}

void sicxe_asm::init() {
	asm_dir["START"] = 0;
	asm_dir["END"] = 0;
	asm_dir["BYTE"] = 1;
	asm_dir["WORD"] = 3;
	asm_dir["RESB"] = 1;
	asm_dir["RESW"] = 3;
	asm_dir["BASE"] = 0;
	asm_dir["NOBASE"] = 0;
	asm_dir["EQU"] = 0;

	reg["A"] = "0"; 
	reg["X"] = "1";
	reg["L"] = "2";
	reg["B"] = "3";
	reg["S"] = "4";
	reg["T"] = "5";
	reg["PC"] = "8";
	reg["SW"] = "9";
}

void sicxe_asm::parse_to_struct(file_parser parser, int index) {
	data.label = parser.get_token(index,0);
	data.opcode = parser.get_token(index,1);
	data.operand = parser.get_token(index,2);
	data.address = format_string(asm_address);
}

void sicxe_asm::first(string filename) {
	file_parser parser(filename);
	parser.read_file();
	assign_address(parser);
}

void sicxe_asm::write_file(string filename) {
	const string columns [] = { "Line#","Address","Label","Opcode","Operand","Machine Code" };
	const string purdylines [] = { "=====", "=======", "=====", "======", "=======", "=======" };
	
	filename = filename.substr(0,filename.length()-4) + ".lis";
	
	myfile.open(filename.c_str());
	
	for (int i = 0; i < 6; i++) 
        myfile << setw(15) << setfill(' ') << columns[i]; 
    myfile << endl;
    
    for (int i = 0; i < 6; i++)
	    myfile << setw(15) << setfill(' ') << purdylines[i];
	myfile << endl;
	
	for (unsigned int i = 0; i < v_data.size(); i++){
        myfile << setw(15) << setfill(' ') << i+1;
        if(ignore_case(v_data[i].label))
            v_data[i].label = "";
        if(ignore_case(v_data[i].opcode))
            v_data[i].opcode = "";
        if(ignore_case(v_data[i].operand))
            v_data[i].operand = "";
        myfile << setw(15) << setfill(' ')  << v_data[i].address;
        myfile << setw(15) << setfill (' ') << v_data[i].label;
        myfile << setw(15) << setfill (' ') << v_data[i].opcode;
        myfile << setw(15) << setfill (' ') << v_data[i].operand;
        myfile << setw(15) << setfill (' ') << v_data[i].machine << endl;   
    }
    myfile.close(); 
}

void sicxe_asm::print_symtab() {
	table.print_table();
}

bool sicxe_asm::check_asm_dir(string s) {
	string temp = upper(s);

	if(asm_dir.find(temp) == asm_dir.end()) {
		return false;
	}
	return true;
}

int sicxe_asm::int_value(string s) {
	if(s[0] == '$' || s[0] == '#' || s[0] == '@') {
		s.erase(0,1);
	}

	int value;
	sscanf(s.c_str(),"%x",&value);
	return value;
}

int sicxe_asm::hex_value(string s) {
	int value;
	if(s[0] == '$' || s[0] == '#' || s[0] == '@') {
		s.erase(0,1);
	}

 	stringstream str_value;
 	str_value << s;
 	str_value >> value;
 	return value;	
}

bool sicxe_asm::ignore_case(string s) {
	if(s[0] == '.' || s.size() == 0) {
		return true;
	}
	return false;
}

string sicxe_asm::upper(string s){
	string tmp = s;
	string::iterator iter;

	for(iter = tmp.begin(); iter != tmp.end(); ++iter) {
  		*iter = std::toupper((unsigned char)*iter);
  	}
	return tmp;
}

string sicxe_asm::format_string(int x) {
	stringstream tmp;
	tmp << hex << setw(5) << setfill('0') << x;
	string xx = tmp.str();
	stringstream tmmp;
	tmmp << setw(5) << xx;
	return upper(tmmp.str());
}

bool sicxe_asm::isAbsolute(string s) {
	if(s[0] == '#')
		return true;
	return false;
}

void sicxe_asm::add_symtab(string address, string label, string operand) {
	if(label[0] != '.' && label.size() != 0) {
		table.add(label, address, isAbsolute(data.operand));
	}
}

// 2nd Pass Functions //
// Main Functions //
void sicxe_asm::second() {
	for(unsigned int i = 1; i < v_data.size(); i++) {
		string op = v_data[i].opcode;
		string operand = v_data[i].operand;
		
		try {
			if(upper(op) == "EQU") {
				table.modify(v_data[i].label, operand, isAbsolute(operand));
				equ[v_data[i].label] = operand;
			}

			if(!check_asm_dir(op) && op.length() != 0 && op[0] != '.') {
				int size = opcode.get_instruction_size(v_data[i].opcode);

				if(size == 1) {
					v_data[i].machine = opcode.get_machine_code(op);
				}
				if(size == 2) {
					v_data[i].machine = format_two(op, operand);
				}

				if(size == 3) {
					string machine_code = get_mach_code(op, operand, v_data[i].address);
					string offset = int_to_hex(get_offset(operand, v_data[i].address),1);
					if(offset.length() > 3){
						offset = offset.substr((offset.length()-3),3);
					}
					if(offset.length() < 3){
						offset = int_to_hex(get_offset(operand, v_data[i].address),3);
					}
					machine_code = machine_code + offset;
					v_data[i].machine = machine_code;
				}

				if(size == 4) {
					string machine_code = get_mach_code(op, operand, v_data[i].address);
					string address = table.get_value(operand);
					machine_code = machine_code + address;
					v_data[i].machine = machine_code;
				}
			}
			else if(upper(op) == "BYTE") {
				if(upper(operand)[0] == 'C' || upper(operand)[0] == 'X') {
					string str = operand.substr(2,operand.find_last_of('\'')-2);
					string machine_code = "";

					if (upper(operand)[0] == 'C'){
						int j;
					   	for(unsigned int n = 0; n < str.length(); n++) {
							j = str[n];
						  	machine_code += int_to_hex(j, 2);
					   	}

					   	v_data[i].machine = machine_code;
				    }
					else {
						v_data[i].machine = str;
					}
				}
			   	else {
			   		v_data[i].machine = int_to_hex(hex_value(operand), operand.length());
			   	}     
			}
			else if(upper(op) == "WORD") {
				v_data[i].machine = int_to_hex(hex_value(operand), 6);
			}
		}
		catch(opcode_error_exception oe) {
			cerr << "An Opcode error has occured on line: " << i+1 << ", " << oe.getMessage() << endl;			
		}
		catch(symtab_exception se) {
			cerr << "An error has occured on line: " << i+1 << ", " << se.getMessage() << endl;			
		}
	}
}

string sicxe_asm::get_mach_code(string op,  string operand, string pc_counter){	
	string mach_opcode = opcode.get_machine_code(op);
	string mach_code = "";
		
	mach_code = mach_opcode[0];
	int second_half_byte = set_ni_bit(operand) + int_value(mach_opcode.substr(1,1));
	mach_code.append(int_to_hex(second_half_byte,1));
	mach_code.append(int_to_hex(set_xbpe_bit(op,operand,pc_counter,base),1));
	
	return mach_code;
}

string sicxe_asm::format_two(string op, string operand) {
	string machine_code = opcode.get_machine_code(op);
	string tmp_opcode = upper(op);
	stringstream str(operand);
	string register_one = "0";
	string register_two = "0";

	getline(str, register_one, ',');
	getline(str, register_two);

	if(tmp_opcode == "SVC") {
		istringstream buffer(register_one);
		int value;
		buffer >> value;
		register_one = int_to_hex(value, 1);
	}
	else if(tmp_opcode == "SHIFTR" || tmp_opcode == "SHIFTL") {
		stringstream value_str;
		int value = int_value(register_two);
		register_one = get_reg_value(register_one);
		value_str << --value;
		register_two = value_str.str();
	}
	else {
		if(check_register(register_one)) {
			register_one = get_reg_value(register_one);
		}
		if(check_register(register_two)) {
			register_two = get_reg_value(register_two);
		}
	}

	machine_code = machine_code + register_one + register_two;
	return machine_code;
}

// SUPPORTING FUNCTIONS //
string sicxe_asm::int_to_hex(int num,int width){
	stringstream out;
	out<<setw(width)<<setfill('0')<<hex<<num;
	//return out.str();
	return upper(out.str());
}

string sicxe_asm::parse_operand(string operand) {
	string temp = operand;
	if(temp[0] == '#' || temp[0] == '@') {
		temp.erase(0,1);
	}

	if(temp.find(',')) {
		stringstream str(temp);
 		getline(str,temp,',');
	}
	return temp;
}

int sicxe_asm::get_offset(string label, string pc_counter) {
	string operand = parse_operand(label);
	string value;

	if(label.size() == 0) {
		value = "0";
	}
	else {
		value = table.get_value(label);
	}

	if(!table.exists(operand)){
		return int_value(value);
	}

	int source = int_value(pc_counter);
	int destination = int_value(table.get_value(operand));
	int offset = destination - (source+3);

	int tmp_base;	
	if(offset >= -2048 && offset <= 2047){
		tmp_base = -1;
	}
	else {
		tmp_base = 1;
	}

	if(tmp_base == 1 && base == 1) {
		return get_base_offset(operand, base_operand);
	}
	else {
		return offset;
	}
}

int sicxe_asm::get_base_offset(string label, string base) {
	int label_address = int_value(table.get_value(label));
	int base_address = int_value(table.get_value(base));
	return (label_address - base_address);
}

bool sicxe_asm::check_register(string r) {
	string rgstr = upper(r);
	if(reg.find(rgstr) != reg.end()) {
		return true;
	}
	return false;
}

string sicxe_asm::get_reg_value(string r) {
	string rgstr = upper(r);
	if(reg.find(rgstr) != reg.end()) {
		reg_iter = reg.find(rgstr);
		return reg_iter->second;
	}
	else {
		throw symtab_exception("INVALID REGISTER.");
	}
}

bool sicxe_asm::is_format4(string opcode){
	return (opcode.find('+') != string::npos);
}

int sicxe_asm::set_ni_bit(string operand) {
	if(operand.size() != 0) {
		if(operand.find('#') != string::npos) {
			return 1;
		}
		else if(operand.find('@') != string::npos) {
			return 2;
		}
		else{
			return 3;
		}
	}
	else {
		return 3;
	}
}

int sicxe_asm::set_xbpe_bit(string opcode,string operand, string pc_counter, int base){
	int tmp_base = base;
	int ni_bit = set_ni_bit(operand);
	bool x_in_operand = (upper(operand).find(",X") != string::npos);
	
	if(operand.size() != 0) {

		int displacement = get_offset(operand,pc_counter);

		if(get_offset(operand,pc_counter) >= -2048 && get_offset(operand,pc_counter) <= 2047){
			tmp_base = -1;
		}
		else if((displacement > 2047 || displacement < -2048) && base == -1 && !is_format4(opcode)) {
			ss << "Displacement is out of range.";
			throw symtab_exception(ss.str());
		}
		else if((displacement > 524287 || displacement < -524288) && base == -1 && is_format4(opcode)){
			ss << "Displacement is out of range.";
			throw symtab_exception(ss.str());
		}
		else {
			tmp_base = 1;
		}

		if(is_format4(opcode)){
			tmp_base = -1;
			if(!x_in_operand) {
				return 1;
			}
			else {
				return 9;
			}
		}
		else {
			string tmp_operand = parse_operand(operand);
			bool base_relative = false;

			if(tmp_base != -1 && base != -1) {
				base_relative = true;
			}

			if(table.exists(tmp_operand)) {
				if(!x_in_operand && base_relative == false){
					return 2;
				}
				else if(!x_in_operand && base_relative != false){
					return 4;
				}
				else if(x_in_operand && base_relative != false){
					return 12;
				}
				else if(base_relative == false && x_in_operand && ni_bit == 3){
					return 10;
				}
				else{
					return 8;
				}
			}
			else {
				return 0;
			}
		}
	}
	else {
		return 0;
	}
}

// Object Funtion
void sicxe_asm::write_object_file(string filename) {
	bool h_flag = true;
	bool t_flag = true;

	int size = 0;
	int next;

	string header = "H";
	string ender = "E";
	string texter = "T";
	string machiner = "";
	string s_address = "";
	string e_address = "";

	vector<string> v_texter;

	for(unsigned int i = 0; i < v_data.size(); i++) {
		string opcode = upper(v_data[i].opcode);

		if(opcode == "START") {
			string label = upper(v_data[i].label);
			if(label.size() == 4) {
				label = label + "  ";
			}
			header = header + label;
		}
		if(opcode == "END") {
			e_address = v_data[i].address;
		}

		if(v_data[i].machine.size() != 0) {
			machiner += v_data[i].machine;
			size += v_data[i].machine.size();
			next = size + v_data[i+1].machine.size();

			if(h_flag) {
				s_address = int_to_hex(int_value(v_data[i].address),6);
				h_flag = false;
			}
			if(t_flag) {
				texter = texter + int_to_hex(int_value(v_data[i].address),6);
				t_flag = false;
			}
			if(next > 60) {
				texter = texter + int_to_hex(machiner.size()/2,2) + machiner;
				v_texter.push_back(texter);
				size = 0;
				t_flag = true;
				machiner = "";
				texter = "T";
			}
		}
		else if(v_data[i].machine.size() == 0 && size >0){
			texter = texter + int_to_hex(machiner.size()/2,2) + machiner;
			v_texter.push_back(texter);
			size = 0;
			t_flag = true;
			machiner = "";
			texter = "T";
		}
	}

	int offset = int_value(e_address) - int_value(s_address);
	header = header + s_address + int_to_hex(offset,6);
	ender = ender + int_to_hex(int_value(s_address),6);

	filename = filename.substr(0,filename.length()-4) + ".o";
	myfile.open(filename.c_str());

	myfile << header << endl;
	for(unsigned int i = 0; i < v_texter.size(); i++) {
		myfile << v_texter[i] << endl;
	}
	myfile << ender << endl;
	myfile.close();
}

string sicxe_asm::get_equ(string symbol) {
	if(equ.find(symbol) == equ.end()) {
		return symbol;
	}
	return get_equ(equ.find(symbol)->second);
}

void sicxe_asm::process_equ() {
	for(equ_iter = equ.begin(); equ_iter != equ.end(); ++equ_iter) {
 		equ[equ_iter->first] = get_equ(equ_iter->first);
 	}
 	print_equ();
}

int main(int argc, char *argv[]) {
 	string file = argv[1];

 	try {
 		sicxe_asm assembler(file);
 	}
 	catch(exception& excpt) {
 		cerr << "An Error has occured: " << excpt.what() << endl;
 	}
 	catch(opcode_error_exception ox) {
 		cerr << "An Opcode error has occured: " << ox.getMessage() << endl;
 	}
 	catch(file_parse_exception fx) {
 		cerr << "A File parser error has occured: " << fx.getMessage() << endl;
 	}
 	catch(symtab_exception sx) {
 		cerr << "An error has occured: " << sx.getMessage() << endl;
 	}
 	catch(...) {
 		cerr << "Uknown failure occured." << endl;
 	}
 	return 0;
}
