#include <xinu.h>
#include <string.h>
#include <stdio.h>

shellcmd xsh_hello(int nargs,  char *args[]){

	if(nargs == 2){
		//only one argument is entered after hello. Printing the success message.
		printf("Hello %s ", args[1]);
		printf("Welcome to the world of XINU!!! \n");
	}else {
		// incorrect number of inputs.. Printing error message.
		if(nargs >2)
		printf("Too many arguments. Please enter only one argument after hello.\n");
		else if(nargs < 2)
		printf("Too less arguments. Please enter exactly one argument after hello.\n");
	}
	return 0;

}
