/* Bryant To, Jessica To
** CS570 - Operating Systems A03*/

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <string>
#include <time.h>

#define NANOPERMIL 1000000
#define MILPERSEC 1000

using namespace std;

// Data to be passed to threads
typedef struct {
	queue<string> *belt;
	struct timespec delay_time;
	int total_produced, frog_cons, escar_cons;
	string person, candy_type;

	/*Belt_Space - available slots on the conveyer belt
	**Candy - available candy on the conveyer belt
	**Mutex - accessing critical section

	**Num_Consumed - total number of candies consumed by both consumers
	**Num_Produced - total number of candies produced by both producers
	**Cbarrier - barrier for consumers
	**Pbarrier - barrier for producers
	**Frog_Count - keeps track of frog candies on belt*/

	sem_t *Belt_Space, *Candy, *Mutex;
	sem_t *Num_Consumed, *Num_Produced, *Cbarrier, *Pbarrier, *Frog_Count;

} VARIABLES;
