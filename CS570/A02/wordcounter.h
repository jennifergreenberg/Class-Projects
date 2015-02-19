/* Bryant To, Jessica To
** CS570 - Operating Systems A02*/

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <sys/stat.h>
using namespace std;

class wordcounter{
public:
	fstream file;
	pthread_t tid;

	typedef struct{
		long CurrentStatus;
		long TerminationValue;
	} PROGRESS_STATUS;

	// Create instance of struct to access values through dot operator
	PROGRESS_STATUS prog_stat;

	struct stat file_stats;

	// Variables for parsing
	long counter;
	char prev_character, character;

	// Default Constructor
	wordcounter(char const *filename);

	// Parse through the file to count the number of words
	long wordcount();

	// Monitors progress of wordcounter task by computing percentage completed
	static void* progress_monitor(void *prog_stat);

private:
	// Spawn thread calling progress_monitor function
	void create_thread();
};
