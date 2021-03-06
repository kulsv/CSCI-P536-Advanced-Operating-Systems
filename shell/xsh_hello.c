#include <xinu.h>
#include <string.h>
#include <stdio.h>

process p1();
process p2(pid32 pid);

process p1(){
	pid32 self_pid = getpid();
	//uint32 self_prio = 40;
//	("priority of p1 before :: %d\n", self_prio);
	uint16 prio2 = resume(create(p2, 1024, 45, "p2",1,self_pid));
	printf("priority of p2 :: %d\n", prio2);
	
}


process p2(pid32 pid1){
	//printf("pid1 is :: %d\n", pid1);
	uint16 prio = chprio(pid1, 21);
	printf("old prio of 1 is :: %d\n", prio);
	
}

shellcmd xsh_hello(int argc,  char *args[]){

	
	uint16 prio1 = resume(create(p1, 1024, 40, "p1",0));
	printf("priority of p1 after :: %d\n", prio1);
	/*if(nargs == 2){
		//only one argument is entered after hello. Printing the success message.
		printf("Hello %s ", args[1]);
		printf("Welcome to the world of XINU!!! \n");
	}else {
		// incorrect number of inputs.. Printing error message.
		if(nargs >2)
		printf("Too many arguments. Please enter only one argument after hello.\n");
		else if(nargs < 2)
		printf("Too less arguments. Please enter exactly one argument after hello.\n");
	}*/

	return SHELL_OK;
}

