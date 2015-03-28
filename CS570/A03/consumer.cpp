/* Bryant To, Jessica To
** CS570 - Operating Systems A03*/

#include "main.h"

void* consumer(void* variable){
	VARIABLES * vars = (VARIABLES *)(variable);
	int total_consumed, barrier_val, total_produced, num_frog;
	string candy_type;

	while(true){
		// Check if the last candy was already taken
		sem_getvalue(vars->Cbarrier, &barrier_val);
		if(barrier_val == 1){
			break;
		}

		sem_wait(vars->Candy);
		
		// ============CRITICAL SECTION START============
		sem_wait(vars->Mutex);
		
		sem_getvalue(vars->Frog_Count, &num_frog);
		sem_getvalue(vars->Num_Produced, &total_produced);
		if(vars->belt->size() > 0 && total_consumed < 100) {
			candy_type = vars->belt->front();
			
			if(candy_type == "crunchy frog bite."){
				vars->frog_cons++;
				sem_wait(vars->Frog_Count);
				sem_getvalue(vars->Frog_Count, &num_frog);
			}
			else
				vars->escar_cons++;
			sem_getvalue(vars->Num_Produced, &total_produced);
			cout << "Belt: " << num_frog << " frogs + ";
			cout << (int)(vars->belt->size() - num_frog - 1) << " escargots" << " = ";
			cout << num_frog + (int)(vars->belt->size() - num_frog - 1) << ".";
			cout << " Produced: " << total_produced;
			cout << "\t" << vars->person << " consumed " << candy_type << endl;

			vars->belt->pop();
			sem_post(vars->Num_Consumed);
			sem_getvalue(vars->Num_Consumed, &total_consumed);
		}
		
		// Last candy has been consumed, set the barrier for the other consumer
		if(total_consumed == 100){
			sem_post(vars->Cbarrier);
		}

		sem_post(vars->Mutex);
		// ============CRITICAL SECTION END============
		
		// Notify producer that candy was taken from the belt
		sem_post(vars->Belt_Space);

		nanosleep(&vars->delay_time, NULL);
	}
	delete vars;
}
