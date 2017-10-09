The details of the data structures and files used are given below: 
  
(1)include/future.h : Future's datatype is declared in this file along with its mode and status. 
  
(2)system/future.c : In this file, all four future operations including system calls are implemented. 
    Following are the details of each function: 
    1> future_alloc(): This function allocates memory to every new future. 
    2> future_free() : This function frees the memory allocated to future and the queues present in the future. 
    3> future_get() : This function is for consumer thread. Three different modes are handled in this.      
                      This function basically checks if future value is ready to be read. I f it is ready, then it allows 
                      consumer thread to read the value. Otherwise it enqueues the thread in get_queue and suspends it. 
    4> future_Set() : This function is for producer thread. Three different modes are handled in this separately. 
                      This function basically allows the producer thread to write the future value if it's not already updated. 
                       
(3) include/f_queue.h : In this file, datatype of queues in future is defined which is a structure that contains  
                        the process id of the thread and pointer to the next element in the queue. This is linked-list  
                        kind of a queue which accesses the elements by traversing through the queue.     
  
(4) system/f_queue.c : In this file, queue operations are defined i.e. enqueue() and dequeue().  
                        These are explained as below: 
    1> f_enqueue() : This function enqueues the new element in the queue on FIFO Basis. 
    2> f_dequeue() : This function removes an element from the queue from the front end of the queue.  
     
(5) apps/future_prodcons.c : In this file, producer and consumer functions are written. 
  
(6) shell/xsh_prodocns.c : In this file, user input is validated and different consumer and producer processes are resumed 
                           to access the future value. 
  
  
Following is the explanation of the code for all three future modes: 
  
(1) FUTURE_EXCLUSIVE: 
    It was given that in this mode, maximum two threads can access the future. Thus, only two processes are allowed. 
    Hence, whenever a consumer thread is resumed, it checks whether the future value is ready or not by checking 
	the state of the future which should be READY. If it is ready, it reads the value of the future otherwise 
	it is suspended for some time until producer writes the future value. 
    When producer thread is resumed, it first checks whether the value is not already written and then writes 
	the value and resumes the consumer thread which is suspended. 
    Consumer thread then resumes and reads the future value and change the future state to EMPTY. 
     
(2) FUTURE_SHARED :  
    It was given that only one producer can write the future value and multiple consumers are allowed. 
    Hence, whenever consumer threads are resumed, it first checks if the future value is ready, if it's not ready, 
	then it gets enqueued in get_queue and suspends itself until the fututre value is not written. 
    Whenever a consumer thread is resumed, it first checks if the future value is not already written and 
	then writes the fuure vale, dequeues all the threads waiting in the get_queue to read the future value and resumes them. 
    Consumer threads when resumed, read the future value and change the future state to EMPTY. 
  
(3) FUTURE_QUEUE : 
    It was given that multiple producers and multiple consumers are alowed. 
    Hence, whenever a consumer thread is resumed, it checks if the future value is alredy written, 
	if yes then reads it and if no, then checks if there are any threads waiting in the set_queue to write the future value.
	If there is any thread waiting, it resumes that thread. That thread writes the future value and consumer reads that. 
	If no thread is waiting to write the future value, it then enqueues itself and suspends itelf. 
    Whenever a producer thread is resumed, it checks if there is any consumer thread waiting to read the value, 
	if yes, then it writes the future value and resumes the consumer thread. If no then it enqueues itself 
	in set_queue and suspends itself. 
     
     
This is how, futures are implemented by making use of suspend() and resume() functions. and synchronization is achieved. 
     
