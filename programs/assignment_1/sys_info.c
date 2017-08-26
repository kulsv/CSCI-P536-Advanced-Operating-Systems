#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
  int main(int argn, char * argv[]) {           		//main function
    int pipe1[2];     						//pipe declaration
    char readArg[100];
    pipe(pipe1);						//pipe called
    int status = fork();
    int exeSuccess;

    if (status > 0) {		
      // parent process functioning..
      printf("Parent PID :: %d\n", getpid());
      close(pipe1[0]);						//closing read end of pipe as no read funtion will be performed by parent process
      write(pipe1[1], argv[1], (strlen(argv[1]) + 1));		//passing command line argument to child process
      close(pipe1[1]);						//closing writing end of the pipe as write operation is done
      wait(NULL);						//waiting for child process to get executed
      exit(0);
    } else {
      if (status == 0) {
	// child process functioning..
        printf("Child PID :: %d\n", getpid());
        close(pipe1[1]);					//closing writing end of pipe in child as no write funtion will be performed by child process
        read(pipe1[0], readArg, 50);    			//reading input from pipe

	//commandline arguments handling for echo

        if (strcmp(readArg, "/bin/echo") != 0) {
         exeSuccess = execl(readArg, readArg, (char * ) NULL);
         if(exeSuccess == -1){
		printf("Incorrect Input. Please enter correct input.\n");		//incorrect input error
		}
        } else {
	//echo execution
          execl("/bin/echo", "echo", "Hello World!", (char * ) NULL);
        }
        close(pipe1[0]); 					// closing reading end of pipe in child as reading is done
      } else {
        printf("Error in Fork call!!");
      }
    }
    return 0;
  }
