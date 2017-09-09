#include <xinu.h>
#include <string.h>
#include <stdio.h>

process polling_proc(int32 *intArray1, int32 *intArray2, int32 *pCurr, int32 *rCurr);
process msg_Pass(pid32 parent_pid);



int32 pCurr = 1;
int32 rCurr = 1;
int32 processArray[];
int32 parentProcess;
int32 currProcess;
int32 nextProcess;
int32 pCount;
int32 rCount;
int32 totalCount;
int32 firstFlag = 1;
int32 printCount;
int32 count = 0;
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
	 printf("before..");
/*******************************************************************************/
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

/***********************************************************************************/
	
        if(option == '2'){
                  //       printf("option 2..");

        	                while(cnt<pCount){

                                proc_pid = create(msg_Pass, 1024, 20, "msgPassing", 1, main_pid);
                                processArray[cnt] = proc_pid;
				printf("process adding :: %d\n", processArray[cnt]);
                                cnt++;
                //                printf("processarray printing..");
                        }
                }
        cnt=0;
/* printf("printing array.. ");
	 while(cnt<pCount){
//		printf("printing array.. ");
		printf("cnt :: %d\n", cnt);
		printf("processArray :: %d\n", processArray[cnt]);
		cnt++;
}
//	currProcess = processArray[0];
		cnt=0;*/
if(option == '1'){
	printf("option 1 selected");
        while(i<=rCount){
                k=1;
                while(k<=pCount){
                        if(cnt<= totalCount){
                                
                                        resume(create(polling_proc, 1024, 20, "pollProc", 4, &intArray[cnt], &intArray[cnt+1], &k, &i));
                               
                                cnt++;
                                k++;
                        }else{
								loopFlag = 1;
                                break;
                                
                        }
                }
				++i;
                if(loopFlag == 1){
                        break;
                }
        }
	}else if(option == '2'){
	/*	printf("in here..");
		 cnt=0;
         while(cnt<pCount){
                //printf();
                printf("cnt:: %d\n", cnt);
                printf("processArray :: %d\n", processArray[cnt]);
                ++cnt;
		
        }*/


		resume(processArray[0]);
		
	}
        for(j=0; j <= totalCount; j++){
                if(intArray[j] >= 0)
                       1;
                }
/*
 cnt=0;
         while(cnt<pCount){
                //printf();
                printf("cnt:: %d\n", cnt);
                printf("processArray :: %d\n", processArray[cnt]);
                ++cnt;
        }
*/
        return SHELL_OK;

}

process msg_Pass(pid32 main_pid){
	int32 cnt=0;
	printf("reached here..");
	cnt=0;
         while(cnt<pCount){
                //printf();
  //              printf("cnt:: %d\n", cnt);
                printf("in processArray :: %d\n", processArray[cnt]);
		++cnt;
	}

	int32 msg;
	if(count < totalCount){
		if(firstFlag == 1){
			parentProcess = 0;
			currProcess = processArray[0];
			printf("first element:: %d\n", processArray[1]);
			nextProcess = processArray[1];
			printf("parentProcess :: %d\n", parentProcess);
			//read and print from printCount  //global
			 printf("currProcess :: %d\n", currProcess);
			 printf("nextProcess :: %d\n", nextProcess);

			printf("valueee :: %d\n", printCount);
			printCount--;
			firstFlag=0;
			send(nextProcess, printCount);
			resume(nextProcess);			
		}else{
			printf("in child call");
			if(pCount == pCurr)
				nextProcess=processArray[0];
			else
				nextProcess = processArray[pCurr];
			
			parentProcess = currProcess;
			currProcess = processArray[pCurr-1];
			  printf("child parentProcess :: %d\n", parentProcess);
			 printf("child currProcess :: %d\n", currProcess);
                         printf("child nextProcess :: %d\n", nextProcess);

			msg = receive();
			send(parentProcess, OK);
			printf("value :: %d\n", msg);
			msg = msg-1;
			send(nextProcess, msg);
			
		}
		if(pCurr<=pCount)
			pCurr++;
		else
			pCurr = 1;
	}
}


process polling_proc(int32 *intArray1, int32 *intArray2, int32 *pCurr, int32 *rCurr){
        int32 currElement = *intArray1;
        int32 nextElement = * intArray2;
        int32 rCnt = *rCurr;
        int32 pCnt = *pCurr;
        nextElement = currElement-1;
        *intArray2 = nextElement;
        printf("Ring Element %d", pCnt);
        printf(" : Round %d", rCnt);
        printf(" : Value : %d\n", currElement);
        return OK;
}

