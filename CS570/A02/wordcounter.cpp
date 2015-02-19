/* Bryant To, Jessica To
** CS570 - Operating Systems A02*/

#include "wordcounter.h"

// Default Constructor
wordcounter::wordcounter(char const *filename){
	file.open(filename);

	if(!file.is_open()){
		cerr << "Could not open file" << endl;
		exit(EXIT_FAILURE);
	}

	// Set TerminationValue of struct
	if(lstat(filename, &file_stats)){
		cerr << "lstat failed to get file status" << endl;
		exit(EXIT_FAILURE);
	}
	prog_stat.TerminationValue = file_stats.st_size;

	counter = 0;
	prev_character = '\0';
}

// Parse through the file to count the number of words
long wordcounter::wordcount(){
	create_thread();

	while(!file.eof()){
		file.get(character);

		prog_stat.CurrentStatus++;
		if(isspace(character) && !isspace(prev_character))
			counter++;
		prev_character = character;
	}
	if(!isspace(character))
	 	counter++;
	 
	pthread_join(tid, NULL);
	free(&tid);
	file.close();
	return counter;
}

// Monitors progress of wordcounter task by computing percentage completed
void* wordcounter::progress_monitor(void *stats){
	PROGRESS_STATUS *pstat = (PROGRESS_STATUS *)(stats);
	float percent_complete;
	int hyphens, hyphen_counter, total_counter, prev;
	prev = 0; hyphen_counter = 0; total_counter = 0;
	
	// Gradually add to the progress bar until the task is completed
	while(percent_complete < 1){
		// Calculate the progress of the task
		percent_complete = (float)pstat->CurrentStatus/pstat->TerminationValue;
		hyphens = (int)(percent_complete*50);

		// Print the progress bar
		while(prev != hyphens && total_counter < 50){
			hyphen_counter++;
			prev++;
			total_counter++;
			cout.flush() << '-';
			if(hyphen_counter%9 == 0){
				cout.flush() << '+';
				total_counter++;
			}
		}		
	}
	cout << endl;
}

// Spawn thread calling progress_monitor function
void wordcounter::create_thread(){
	if(pthread_create(&tid, NULL, progress_monitor, (void *)&prog_stat)){
		cerr << "Failed to create thread" << endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[]){
	long count;

	// Check user input for file name
	if(argc == 1){
		cerr << "No file specified" << endl;
		exit(EXIT_FAILURE);
	}
	if(argc > 2){
		cerr << "Too many files specified" << endl;
		exit(EXIT_FAILURE);
	}

	wordcounter *word_count = new wordcounter(argv[1]);
	count = word_count->wordcount();
	cout << count << " words" << endl;
	return 0;
}
