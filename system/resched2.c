#include <xinu.h>

struct  defer   Defer;
void    resched2(
        pid32 pid, uint16 next_state
)           /* Assumes interrupts are disabled      */
{
        struct procent *ptold;  /* Ptr to table entry for old process   */
        struct procent *ptnew;  /* Ptr to table entry for new process   */
        struct procent *pt;
        /* If rescheduling is deferred, record attempt and return */

        if (Defer.ndefers > 0) {
                Defer.attempt = TRUE;
                return;
        }


        if(next_state < 0 || next_state > 7){
                return;
        }

        pt = &proctab[pid];

        if (pt->prstate == PR_CURR) {  /* Process remains eligible */
                if (pt->prprio > firstkey(readylist)) {
                        return;
                }

                /* Old process will no longer remain current */

                pt->prstate = PR_READY;
                insert(currpid, readylist, pt->prprio);
        }else{
              	pt->prstate = next_state;
        }


	/* Point to process table entry for the current (old) process */

        ptold = &proctab[currpid];

        if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
                if (ptold->prprio > firstkey(readylist)) {
                        return;
                }

                /* Old process will no longer remain current */

                ptold->prstate = PR_READY;
                insert(currpid, readylist, ptold->prprio);
        }
	/* Force context switch to highest priority ready process */
  currpid = dequeue(readylist);
        ptnew = &proctab[currpid];
        ptnew->prstate = PR_CURR;
        preempt = QUANTUM;              /* Reset time slice for process */

#ifdef MMU
	FlushTLB();
        setPageTable();
#endif/*MMU*/

	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

        /* Old process returns here when resumed */

        return;
}


