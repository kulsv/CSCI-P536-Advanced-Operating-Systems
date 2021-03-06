#include <xinu.h>
#include <future.h>

uint future_prod(future_t* fut,int n) {
	if(fut !=NULL){
		printf("Produced %d\n",n);
		future_set(fut, n);
	}else{
		printf("Cannot Set Future Value\n");
	}
	return OK;
}

uint future_cons(future_t* fut) {
	int i, status;
	if(fut !=NULL){
		status = (int)future_get(fut, &i);
		if (status < 1) {
			printf("future_get failed\n");
			return -1;
		}
		printf("Consumed %d\n", i);
	}else{
		printf("Cannot Get Future Value\n");
	}
	return OK;
}




