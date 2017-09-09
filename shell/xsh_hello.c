#include <xinu.h>
#include <string.h>
#include <stdio.h>

shellcmd xsh_hello(int argc,  char *args[]){

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

	int32 p = 2; 
	char *endp;
	int32 z;
	for(z = 1; z < argc; z++){
		if (0 == strncmp("-p", args[z], 3)){
			if (! (z + 1 < argc)) {
				print_usage();
				printf("-p flag expected an argument\n");
				return SHELL_ERROR;
			}
			p = sscanf(args[z + 1], &endp, 10);
			if (endp == args[z + 1]) {
				print_usage();
				printf("-p flag expected an integer\n");
				return SHELL_ERROR;
			} else if (*endp != '\0'){
				print_usage();
				printf("-p recieved invalid integer\n");
				return SHELL_ERROR;
			} else if (!(0 <= p && p <= 64)) {
				print_usage();
				printf("-p flag expected a number between 0 - 64\n");
				return SHELL_ERROR;
			}
			z += 1; 
	}			}
	printf("argument  is %d\n", p);
	return SHELL_OK;
}


void print_usage(void){
  printf("Usage: ...\n");
}
