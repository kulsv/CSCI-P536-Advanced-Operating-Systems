#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <process_ring.h>

process polling_proc(int32 *intArray1, int32 *intArray2, int32 *pCurr, int32 *rCurr);
process msg_Passing(pid32 main_pid, pid32 parent_pid, int32 *next_pid);

shellcmd xsh_pRing(int nargs, char *args[]){
        int32 pCount = 3;
        int32 rCount = 5;
        int32 totalCount = pCount*rCount - 1;
        int32 printCount = totalCount;
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
        int32 processArray[pCount];
		int32 firstFlag = 1;
		pid32 parent_pid = 0;
		int32 next_pid = 0;
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
           		 while(cnt<pCount){
				//printf("entering loop.. %d\n", cnt);
				if(firstFlag == 1){
					parent_pid = 0;
					//printf("first parentid :: %d\n", parent_pid);
					firstFlag = 0;
				}else {
					 //printf("processArray[cnt-1] :: %d\n", processArray[cnt-1]);

					parent_pid = processArray[cnt-1];
					 //printf("parentid :: %d\n", parent_pid);

				}
				if(cnt == pCount-1){
				//	printf("address of 0 :: %d\n", &processArray[0]);
					next_pid = &processArray[0];
//					 printf("next_pid :: %d\n",&processArray[0]);

				}else{
					
				//	printf("next_pid :: %d\n",&(processArray[cnt+1]));
 				//	printf("cnt here .. :: %d\n", cnt);
					next_pid = &(processArray[cnt+1]);
				//	 printf("cnt here 2.. :: %d\n", cnt);	
				}
				
                		proc_pid = create(msg_Passing, 1024, 20, "msgPassing", 3, main_pid, parent_pid, next_pid);
                		processArray[cnt] = proc_pid;
//	                	printf("process adding :: %d\n", processArray[cnt]);
        	        	cnt++;
                		//printf("processarray printing.. %d\n", cnt);
            		}
			cnt = 0;
			send(processArray[0], printCount);		
			while(cnt<pCount){
//				printf("id:: %d\n", processArray[cnt]);
//				printf("address:: %d\n", &processArray[cnt]);
				resume(processArray[cnt]);
				cnt++;
            		}
			//cnt = 0;

        }
		
	
	}
	
	
	
process msg_Passing(pid32 main_pid, pid32 parent_pid, int32 *next_pid){

//	printf("in msg_Pass..");
	int nextAddr;
	nextAddr = *next_pid;
	printf("currPid :: %d\n", getpid());
	//printf("next_pid :: %d\n",next_pid);

	printf("next_pid :: %d\n",nextAddr);

	int32 msg = receive();
	send(main_pid, OK);
	printf("value :: %d\n", msg);
	msg--;
	if(msg != -1){
	
		send(nextAddr, msg);
		resume(nextAddr);
	}else{
		send(main_pid, OK);
	}
	return OK;

}

