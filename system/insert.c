/* insert.c - insert */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  insert  -  Insert a process into a queue in descending key order
 *------------------------------------------------------------------------
 */
status	insert(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Key for the inserted process	*/
	)
{
											/* Runs through items in a queue*/
	struct qentry	*prev;			/* Holds previous node index	*/
	struct qentry	*curr;
	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	curr = &queuetab[firstid(q)];
	//kprintf("Before WHile PId %d QID %d KEY %d \n",pid,q,key);
	while (curr != NULL && queuetab[curr->q_pid].qkey >= key) {
		curr = queuetab[curr->q_pid].qnext;
		//kprintf("Inside WHile \n");

	}

	/* Insert process between curr node and previous node */

	prev = curr->qprev;	/* Get index of previous node	*/
	queuetab[pid].qnext = curr;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[pid].q_pid = pid;
	prev->qnext = &queuetab[pid];
	curr->qprev = &queuetab[pid];
	return OK;
}
