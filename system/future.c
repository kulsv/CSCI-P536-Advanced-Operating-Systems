#include <xinu.h>
#include <future.h>
#include <f_queue.h>

pid32 f_enqueue (pid32, struct fqentry*);
pid32 f_dequeue (struct fqentry*);


future_t* future_alloc(future_mode_t mode){
	future_t* f_ptr =  (struct fqentry*)getmem(sizeof(future_t));						//allocating future
	f_ptr->mode = mode;
	f_ptr->state = FUTURE_EMPTY;
	f_ptr->value = 0;
	f_ptr->set_queue =  (struct fqentry*)getmem (sizeof (struct fqentry));			//allocating set_queue
	f_ptr->set_queue->fqnext = NULL;

	f_ptr->get_queue =  (struct fqentry*)getmem (sizeof (struct fqentry));			//allocating get_queue
	f_ptr->get_queue->fqnext = NULL;

	return f_ptr;
}


syscall future_free(future_t* f){
	while (f->get_queue->fqnext != NULL)
	{
		struct fqentry* temp = f->get_queue;
		f->get_queue = f->get_queue->fqnext;		
		freemem((char *)(temp), sizeof(struct fqentry));
	}

	while (f->set_queue->fqnext != NULL)
	{
		struct fqentry* temp = f->set_queue;
                f->set_queue = f->set_queue->fqnext;
		freemem((char *)(temp), sizeof(struct fqentry));
	}
	freemem(&f ,sizeof(future_t));
	return OK;
}

syscall future_get(future_t* f_ptr, int* n){
	intmask mask;
	mask = disable();
	if(f_ptr->state != FUTURE_READY){						//Read future value only if future value is READY
		f_ptr->state = FUTURE_WAITING;
		if(f_ptr->mode == FUTURE_EXCLUSIVE){					// Handling first mode
			if(f_ptr->pid > 0)
				return SYSERR;						//more than one consumers are not allowed
			f_ptr->pid = getpid();						//storing pid of waiting thread in future's pid field
			suspend(getpid());
			*n = f_ptr->value;						//reading future value after resuming
			f_ptr->state = FUTURE_EMPTY;
		}else if(f_ptr->mode == FUTURE_SHARED){					// handling SHARED mode
			f_enqueue(getpid(), f_ptr->get_queue);				// enqueuing consumer thread in get_queue and suspending it 
			suspend(getpid());
			*n = f_ptr->value;						//reading value after resuming consumer thread
			if(f_ptr->get_queue->fqnext == NULL){
				f_ptr->state = FUTURE_EMPTY;				// changing future state to empty as no more threads in wait queue	
			}
		}else if(f_ptr->mode == FUTURE_QUEUE){					// Handling QUEUE mode
			struct fqentry *squeue = f_ptr->set_queue;
			if(squeue->pid !=0){						// checking set_queue
				struct fqentry* temp =f_ptr->get_queue->fqnext;
				pid32 pid = f_dequeue(f_ptr->set_queue);		// dequeuing one element from set_queue
				resume(pid);
				f_ptr->get_queue->pid = temp->pid;
                                f_ptr->get_queue->fqnext = temp->fqnext;
			}else{
				//ENQUEUE ITSELF
				f_enqueue(getpid(), f_ptr->get_queue);			// no thread waiting in set_queue hence enqueuing itself and suspending
				f_ptr->state = FUTURE_WAITING;
				suspend(getpid());
			}
			*n = f_ptr->value;						//reading future value and handling future status
			//check state
			if(f_ptr->get_queue ==NULL || f_ptr->get_queue->pid != 0){
				//other threads waiting in get_queue
				f_ptr->state = FUTURE_WAITING;
			}else{
				f_ptr->state = FUTURE_EMPTY;
			}
		}
	}else {
		*n = f_ptr->value;
	}


	restore(mask);
	return OK;
}


syscall future_set(future_t* f_ptr, int n){
	intmask mask;
	mask = disable();
	if(f_ptr->state == FUTURE_EMPTY || f_ptr->state == FUTURE_WAITING){			//Write into future value only if future is in empty or waiting state
		if(f_ptr->mode == FUTURE_EXCLUSIVE){						// handling future exclusive mode			
			if(f_ptr->set_queue->pid > 0){						// allowing only one producer to write future value
				printf("Cannot Set Future Value.. \n");
				return SYSERR;
			}
			f_ptr->value = n;							//writing future value
			f_ptr->state = FUTURE_READY;
			f_ptr->set_queue->pid = getpid();
			f_ptr->set_queue->fqnext = NULL;
			resume(f_ptr->pid);							//resuming thread from get_queue to read future value
		}else if(f_ptr->mode == FUTURE_SHARED){						//handling future shared state				
			if(f_ptr->set_queue->pid > 0){
				printf("Cannot Set Future Value.. \n");				// already one producer has produced future value
				return SYSERR;
			}
			f_ptr->value = n;							//writing future value				

			f_ptr->state = FUTURE_READY;
			f_ptr->set_queue->pid = getpid();
			f_ptr->set_queue->fqnext = NULL;
			while(f_ptr->get_queue->pid != 0){
				//pid32 pid = f_dequeue(f_ptr->get_queue, f_ptr->mode);		//resuming all threads from get_queue to read future value
				struct fqentry* temp =f_ptr->get_queue->fqnext;
				pid32 pid = f_dequeue(f_ptr->get_queue);
				resume(pid);
				f_ptr->get_queue->pid = temp->pid;
				f_ptr->get_queue->fqnext = temp->fqnext;
				//f_ptr->get_queue = f_ptr->get_queue->fqnext;
			
			}

		}else if(f_ptr->mode == FUTURE_QUEUE){						//handling future queue state	
			struct fqentry *gqueue = f_ptr->get_queue;
			if(gqueue->pid !=0){							// checking get_queue
				f_ptr->value = n;														
				f_ptr->state = FUTURE_READY;
				struct fqentry* temp =f_ptr->get_queue->fqnext;	

				pid32 pid = f_dequeue(f_ptr->get_queue);			//dequeuing thread from get_queue to read future value
				f_ptr->get_queue->pid = temp->pid;
                                f_ptr->get_queue->fqnext = temp->fqnext;

				resume(pid);


			}else{
				//ENQUEUE ITSELF
				f_enqueue(getpid(), f_ptr->set_queue);				// no thread waiting to read future value hence enqueuing itself
				f_ptr->state = FUTURE_WAITING;
				suspend(getpid());
			}

		}
	}else{
		restore(mask);
		return SYSERR;
	}
	restore(mask);
	return OK;
}

