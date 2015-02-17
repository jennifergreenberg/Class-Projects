#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <dirent.h>
#include <cstdlib>
using namespace std;

class UnixLS{
public:
	// Constructor
	UnixLS(int argc, const char **args);

	// Opens and reads the appropriate directories
	void handleDir();

private:
	DIR *directory;

	// Stores number of arguments in input
	int size;

	// Flags for hidden files and whether directories have been specified
	bool readHidden;
	bool specDir;

	// Vector data structure holding delimited input arguments
	vector<string> arg;

	// Check input arguments and sets boolean flags
	void checkArguments();

	// Prints the files found in specified directories
	void printFiles();

	// Directory error handling
	void checkDir(int i);
};
