/* Bryant To, Jessica To
** CS570 - Operating Systems A03*/

#include "main.h"

void* producer(void* variable){
	VARIABLES * vars = (VARIABLES *)(variable);
	int total_produced, barrier_val, num_frog;

	while(true){
		// Check if the last candy has already been produced
		sem_getvalue(vars->Pbarrier,&barrier_val);
		if(barrier_val == 1){
			break;
		}

		sem_wait(vars->Belt_Space);

		nanosleep(&vars->delay_time, NULL);

		// ============CRITICAL SECTION START============
		sem_wait(vars->Mutex);

		// Check for frog candies on belt

		sem_getvalue(vars->Frog_Count, &num_frog);
		if(vars->candy_type == "crunchy frog bite." && num_frog == 3)
			sem_post(vars->Mutex);

		else{
			if(vars->belt->size() < 10 && total_produced < 99){
				vars->belt->push(vars->candy_type);
				vars->total_produced++;
				sem_post(vars->Num_Produced);
				if(vars->candy_type == "crunchy frog bite."){
					sem_post(vars->Frog_Count);
					sem_getvalue(vars->Frog_Count, &num_frog);
				}
				sem_getvalue(vars->Num_Produced,&total_produced);
				cout << "Belt: " << num_frog << " frogs + ";
				cout << abs((int)(vars->belt->size() - num_frog)) << " escargots" << " = ";
				cout << num_frog + abs((int)(vars->belt->size() - num_frog)) << ".";
				cout << " Produced: " << total_produced;
				cout << "\tAdded " << vars->candy_type << endl;
			}
			
			// Last candy has been produced, set the barrier for the other producer
			if(total_produced == 100){
				sem_post(vars->Pbarrier);
			}

			sem_post(vars->Mutex);
		}
		// ============CRITICAL SECTION END============

		// Notify consumer that there is candy on the belt
		sem_post(vars->Candy);
	}
	delete vars;
}
