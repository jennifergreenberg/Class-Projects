#include "UnixLS.h"
using namespace std;

int main(int argc, const char *argv[])
{
	UnixLS *list = new UnixLS(argc, argv);
	list->handleDir();

	delete list;

	return 0;
}
