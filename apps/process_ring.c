#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <process_ring.h>


process msg_Pass(pid32 main_pid){
        int32 cnt=0;
        printf("reached here..");
        cnt=0;
         while(cnt<pCount){
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

                         printf("Ring Element %d", pCnt);
                        printf(" : Round %d", rCnt);
                        printf(" : Value : %d\n", currElement);
                                                        //read and print from printCount  //global

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
                         printf("Ring Element %d", pCnt);
                        printf(" : Round %d", rCnt);
                        printf(" : Value : %d\n", currElement);

                        msg = receive();
                        send(parentProcess, OK);
                //      printf("value :: %d\n", msg);
                        msg = msg-1;
                        send(nextProcess, msg);

                }
	}
        if(pCurr<=pCount)
               pCurr++;
        else
               pCurr = 1;

}


process polling_proc(int32 *intArray1, int32 *intArray2, int32 *pCurr, int32 *rCurr){
        int32 currElement = *intArray1;
        int32 nextElement = * intArray2;
        int32 rCnt = *rCurr;
        int32 pCnt = *pCurr;
        nextElement = currElement-1;					//getting next element
        *intArray2 = nextElement;
        printf("Ring Element %d", pCnt);
        printf(" : Round %d", rCnt);
        printf(" : Value : %d\n", currElement);
        return OK;
}


