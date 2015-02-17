#include "UnixLS.h"

struct dirent *files;

UnixLS::UnixLS(int argc, const char **args){
	size = argc;
	for(int i = 0; i < size; i++){
		arg.push_back(args[i]);
	}

	readHidden = false;
	specDir = false;
}

// Check input arguments and sets boolean flags
void UnixLS::checkArguments(){
	if(size > 1){
		if(arg[1] == "-h"){
			readHidden = true;
			if(size > 2){
				specDir = true;
			}
		} 
		if(!readHidden){
			specDir = true;
		}
	}

}

// Prints the files found in specified directories
void UnixLS::printFiles(){
	if(readHidden){
		cout << files->d_name << endl;
	}
	else{
		if(files->d_name[0] != '.'){
			cout << files->d_name << endl;
		}
	}
}

// Directory error handling
void UnixLS::checkDir(int i){
	if(directory == NULL){
		cerr << "Cannot access " + arg[i] << endl;
		exit(EXIT_FAILURE); 
	}
}

// Opens and reads the appropriate directories
void UnixLS::handleDir(){
	checkArguments();
	
	if(specDir){
		if(readHidden){
			for(int i = 2; i < size; i++){
				directory = opendir(arg[i].c_str());
				checkDir(i);
				while(files = readdir(directory)){
					printFiles();
				}
			}
		}
		else{
			for(int i = 1; i < size; i++){
				directory = opendir(arg[i].c_str());
				checkDir(i);
				while(files = readdir(directory)){
					printFiles();
				}
			}
		}
	}

	// Default listing of current directory if no directories were specified
	else{
		directory = opendir(".");
		while(files = readdir(directory)){
			printFiles();
		}
	}
}
