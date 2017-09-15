#include <xinu.h>
#include <process_ring.h>
process msg_Passing(pid32 main_pid, pid32 parent_pid, int32 *next_pid, int32 *rCurr, int32 pCurr){
        int nextAddr;
        int32 round = *rCurr;
        nextAddr = *next_pid;
        int32 msg;
        while(round <= rCount){
                msg = receive();
                //printf("value :: %d\n", msg);
                printf("Ring Element %d", pCurr);
                printf(" : Round %d", round);
                printf(" : Value : %d\n", msg);
                send(main_pid, OK);
                msg--;
                if(msg != -1){
                        send(nextAddr, msg);
                }
                round++;
        }
        return OK;
}


process polling_proc(int32 *intArray1, int32 *intArray2, int32 *pCurr, int32 *rCurr){
        int32 currElement = *intArray1;
        int32 nextElement = *intArray2;
        int32 rCnt = *rCurr;
        int32 pCnt = *pCurr;
        nextElement = currElement - 1;
        *intArray2 = nextElement;
        printf("Ring Element %d", pCnt);
        printf(" : Round %d", rCnt);
        printf(" : Value : %d\n", currElement);
        return OK;
}


