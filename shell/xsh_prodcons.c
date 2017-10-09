#include <xinu.h>
#include <future.h>
#include <f_queue.h>

uint future_prod(future_t*,int);
uint future_cons(future_t*);

shellcmd xsh_prodcons(int nargs, char *args[]){

	if(nargs == 2 && strcmp(args[1], "-f") == 0) {
		future_t* f_exclusive;
		f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
		future_t* f_shared;
		f_shared    = future_alloc(FUTURE_SHARED);
		future_t* f_queue;
		f_queue     = future_alloc(FUTURE_QUEUE);

		// Test FUTURE_EXCLUSIVE
		resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
		resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 1) );


		// Test FUTURE_SHARED
		resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
		resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 2) );


		// Test FUTURE_QUEUE
		resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
		resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, 3) );
		resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, 4) );
		resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, 5) );
		resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, 6) );

//		future_free(f_exclusive);
//		future_free(f_shared);
//		future_free(f_queue);
		sleep(0.5);
		future_free(f_exclusive);
                future_free(f_shared);
                future_free(f_queue);

		return SHELL_OK;
	}else
		printf("Please provide valid input");
	return SHELL_OK;
}




