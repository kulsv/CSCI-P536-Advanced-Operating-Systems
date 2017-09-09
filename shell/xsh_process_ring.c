#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <process_ring.h>


shellcmd xsh_process_ring(int argc, char *args[]){
         pCount = 3;
         rCount = 5;
         totalCount = pCount*rCount - 1;
        printCount = totalCount;
        int32 intArray[totalCount];
        int32 i = 1;
        int32 k = 1;
        int32 cnt = 0;
        intArray[0] = totalCount;
        int32 j =0;
        int32 option = '2';
        pid32 proc_pid = 0;
        pid32 main_pid = getpid();
        int32 loopFlag = 0;
        int32 prev_procId;
        processArray[pCount];

	cnt=0;
//	 printf("before..");
	 int32 p = 2;
        char *endp;
        int32 z;
        for(z = 1; z < argc; z++){
                if (0 == strncmp("-p", args[i], 3)){				
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
        }                       }
        printf("argument  is %d\n", p);

	
        if(option == '2'){								//synchronization handling
                  //       printf("option 2..");

        	                while(cnt<pCount){

                                proc_pid = create(msg_Pass, 1024, 20, "msgPassing", 1, main_pid);		//creating processes
                                processArray[cnt] = proc_pid;
				printf("process adding :: %d\n", processArray[cnt]);
                                cnt++;
                //                printf("processarray printing..");
                        }
                }
        cnt=0;
	 while(cnt<pCount){
//		printf("printing array.. ");
		printf("cnt :: %d\n", cnt);
		printf("processArray :: %d\n", processArray[cnt]);
		cnt++;
}
//	currProcess = processArray[0];
		cnt=0;*/
if(option == '1'){								//checking first condition
	printf("option 1 selected");
        while(i<=rCount){
                k=1;
                while(k<=pCount){
                        if(cnt<= totalCount){
                                
                                resume(create(polling_proc, 1024, 20, "pollProc", 4, &intArray[cnt], &intArray[cnt+1], &k, &i)); // resuming polling proc
                               
                                cnt++;
                                k++;
                        }else{
				loopFlag = 1;					//last value is reached
                                break;
                                
                        }
                }
				++i;
                if(loopFlag == 1){
                        break;
                }
        }
	}else if(option == '2'){
		resume(processArray[0]);
		
	}
 	       for(j=0; j <= totalCount; j++){                                  //checking if message passing is done
        	        if(intArray[j] >= 0)
                	       1;
                }

         
        return SHELL_OK;

}
