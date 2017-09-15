#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <process_ring.h>
shellcmd xsh_process_ring(int nargs, char *args[]){
        int32 pCount = 2;
        rCount = 3;
        int32 i = 1;
        int32 k = 1;
        int32 cnt = 0;
        int32 j =0;
        int32 option;// = '2';
        pid32 proc_pid = 0;
        pid32 main_pid = getpid();
        int32 loopFlag = 0;
        int32 prev_procId;
        int32 firstFlag = 1;
        pid32 parent_pid = 0;
        int32 next_pid = 0;
        int32 itrt=0;
        int32 nxtItrt = 0;
        int32 ret;
        int32 inputFlag=0;
        int32 rCurr= 1;
 for(itrt=1; itrt<=nargs; itrt+=2){
                        nxtItrt = itrt+1;
                        if(strcmp(args[itrt], "-p") == 0){
                                int32 num;
                                num = atoi(args[itrt+1]);
                                if(num < 1 || num > 64){
                                        printf("Please enter a valid input between 1 and 64..");
                                }else{
                                        pCount=num;
                                }
                        }
 if(strcmp(args[itrt], "-r") == 0){
                                int32 num2;
                                num2 = atoi(args[itrt+1]);
                               if(num2 == 0){
                                        printf("Please enter a valid value for r");
                                }else{
                                        rCount = num2;
                                }
                        }

                         if(strcmp(args[itrt], "-i") == 0){
                               if(!(strcmp(args[itrt+1], "poll") == 0) && !(strcmp(args[itrt+1], "sync") == 0)){
                                      printf("Please provide some valid input..");
                                }else{
                                        inputFlag = 1;
                                        if(strcmp(args[itrt+1], "poll") == 0){
                                                option = '1';
                                        }else if(strcmp(args[itrt+1], "sync") == 0){
                                                option = '2';
                                        }
                                }
                        }
                }
 if(inputFlag == 0)
                        printf("Please provide some input..");

                int32 totalCount = pCount*rCount - 1;
                int32 printCount = totalCount;
                int32 intArray[totalCount];
                intArray[0] = totalCount;
                int32 processArray[pCount];
 if(option == '1'){
                                        while(i<=rCount){
                                                        k=1;
                                                   while(k<=pCount){
                                                                if(cnt<= totalCount){
                                                                                resume(create(polling_proc, 1024, 20, "pollProc", 4, &intArray[cnt],
											 &intArray[cnt+1], &i, &k));
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
                                if(firstFlag == 1){
                                        parent_pid = 0;
                                        firstFlag = 0;
                                }else {
                                        parent_pid = processArray[cnt-1];
                                }
                                if(cnt == pCount-1){
                                        next_pid = &processArray[0];
                                }else{
                                        next_pid = &(processArray[cnt+1]);
                                }
                                proc_pid = create(msg_Passing, 1024, 20, "msgPassing", 5, main_pid, parent_pid, next_pid, &rCurr, cnt+1);
                                processArray[cnt] = proc_pid;
                                rCurr;
                                cnt++;
                        }
                        cnt = 0;
                        send(processArray[0], printCount);
                        while(cnt<pCount){
                                resume(processArray[cnt]);
                                cnt++;
                        }

                }
                              return SHELL_OK;
}

