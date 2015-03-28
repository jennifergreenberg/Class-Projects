/* Bryant To, Jessica To
** CS570 - Operating Systems A03*/

#include "main.h"

void* producer(void* variable);
void* consumer(void* variable);

int main(int argc, char **argv)
{
	VARIABLES frog, escar, ethel, lucy;
	queue<string> con_belt;
	int arguments = 0;
	sem_t belt_Space, candy, mutex, num_pro, pbar, fcount, num_con, cbar, sepbar;
	pthread_t frog_thread, escar_thread, ethel_thread, lucy_thread;
	
	// Declare struct variables - QUEUE
	frog.belt = escar.belt = ethel.belt = lucy.belt = &con_belt;
	
	// Declare struct variables - INTEGERS
	frog.total_produced = escar.total_produced = 0;
	ethel.frog_cons = ethel.escar_cons = lucy.frog_cons = lucy.escar_cons = 0;
	
	// Declare struct variables - STRINGS
	frog.candy_type = "crunchy frog bite.";
	escar.candy_type = "escargot sucker.";
	ethel.person = "Ethel";
	lucy.person = "Lucy";
	frog.person = "frog producer";
	escar.person = "escar producer";

	// Declare struct variables - SEMAPHORES
	frog.Belt_Space=escar.Belt_Space=ethel.Belt_Space=lucy.Belt_Space = &belt_Space;
	frog.Candy=escar.Candy=ethel.Candy=lucy.Candy = &candy;
	frog.Mutex=escar.Mutex=ethel.Mutex=lucy.Mutex = &mutex;
	frog.Num_Produced=escar.Num_Produced=ethel.Num_Produced=lucy.Num_Produced = &num_pro;
	frog.Pbarrier=escar.Pbarrier = &pbar;
	frog.Frog_Count=escar.Frog_Count=ethel.Frog_Count=lucy.Frog_Count = &fcount;
	ethel.Num_Consumed=lucy.Num_Consumed = &num_con;
	ethel.Cbarrier=lucy.Cbarrier = &cbar;

	// Process command line arguments
	while((arguments = getopt(argc, argv, "E:L:f:e:")) != EOF){
		switch(arguments){
			case 'E':
				ethel.delay_time.tv_sec = atoi(optarg) / MILPERSEC;
				ethel.delay_time.tv_nsec = (atoi(optarg) % MILPERSEC) * NANOPERMIL;
				break;
			case 'L':
				lucy.delay_time.tv_sec = atoi(optarg) / MILPERSEC;
				lucy.delay_time.tv_nsec = (atoi(optarg) % MILPERSEC) * NANOPERMIL;
				break;
			case 'f':
				frog.delay_time.tv_sec = atoi(optarg) / MILPERSEC;
				frog.delay_time.tv_nsec = (atoi(optarg) % MILPERSEC) * NANOPERMIL;
				break;
			case 'e':
				escar.delay_time.tv_sec = atoi(optarg) / MILPERSEC;
				escar.delay_time.tv_nsec = (atoi(optarg) % MILPERSEC) * NANOPERMIL;
				break;
		}
	}

	// ==================== Initialize semaphores ====================
	if (sem_init(&belt_Space, 0, 10) == -1){
		cerr << "Failed to initialize belt_Space semaphore" << endl;
		exit(EXIT_FAILURE);}
	if (sem_init(&candy, 0, 1) == -1){
		cerr << "Failed to initialize candy semaphore" << endl;
		exit(EXIT_FAILURE);}
	if (sem_init(&mutex, 0, 1) == -1){
		cerr << "Failed to initialize mutex semaphore" << endl;
		exit(EXIT_FAILURE);}

	if (sem_init(&num_pro, 0, 0) == -1){
		cerr << "Failed to initialize num_produced semaphore" << endl;
		exit(EXIT_FAILURE);}
	if (sem_init(&pbar, 0, 0) == -1){
		cerr << "Failed to initialize producer barrier semaphore" << endl;
		exit(EXIT_FAILURE);}
	if(sem_init(&fcount, 0, 0) == -1){
		cerr << "Failed to initialize frog count semaphore" << endl;
		exit(EXIT_FAILURE);}
	if (sem_init(&num_con, 0, 0) == -1){
		cerr << "Failed to initialize num_consumed semaphore" << endl;
		exit(EXIT_FAILURE);}
	if (sem_init(&cbar, 0, 0) == -1){
		cerr << "Failed to initialize consumer barrier semaphore" << endl;
		exit(EXIT_FAILURE);}
	
	// ==================== Create threads ====================
	if(pthread_create(&frog_thread, NULL, producer, (void *)&frog)){
		cerr << "Failed to create producer thread" << endl;
		exit(EXIT_FAILURE);}
	if(pthread_create(&escar_thread, NULL, producer, (void *)&escar)){
	 	cerr << "Failed to create thread" << endl;
	 	exit(EXIT_FAILURE);}
	if(pthread_create(&ethel_thread, NULL, consumer, (void *)&ethel)){
		cerr << "Failed to create ethel thread" << endl;
		exit(EXIT_FAILURE);}
	if(pthread_create(&lucy_thread, NULL, consumer, (void *)&lucy)){
		cerr << "Failed to create lucy thread" << endl;
		exit(EXIT_FAILURE);}

	// ==================== Wait for threads to finish ====================
	pthread_join(frog_thread, NULL);
	pthread_join(escar_thread, NULL);
	pthread_join(ethel_thread, NULL); 
	pthread_join(lucy_thread, NULL);

	cout << "\nPRODUCTION REPORT\n---------------------------------------" << endl; 
	cout << "crunchy frog bite producer generated " << frog.total_produced;
	cout << " candies" << endl;
	cout << "escargot sucker producer generated " << escar.total_produced;
	cout << " candies" << endl;
	cout << "Lucy consumed " << lucy.frog_cons << " crunchy frog bites + ";
	cout << lucy.escar_cons << " escargot suckers = ";
	cout << lucy.frog_cons + lucy.escar_cons << endl;
	cout << "Ethel consumed " << ethel.frog_cons << " crunchy frog bites + ";
	cout << ethel.escar_cons << " escargot suckers = ";
	cout << ethel.frog_cons + ethel.escar_cons << endl;
	return 0;
}
