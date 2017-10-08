#include <xinu.h>
#include <f_queue.h>
#include <future.h>
pid32 f_enqueue (pid32 pid, struct fqentry *fqueue)
{
	struct fqentry *temp;
	struct fqentry *tail;
	struct fqentry *newfq;
	temp = fqueue;
	tail = fqueue;
	newfq = (struct qentry*)(getmem (sizeof (struct fqentry)));			//creating new entry in queue
	newfq->pid = pid;
	newfq->fqnext = NULL;
	while (temp != NULL)
	{
		if (temp->fqnext == NULL)						//traversing to findthe last element of queue	
		{
			tail = temp;
			break;
		}
		else
		{
			temp = temp->fqnext;
		}
	}
	if (tail->pid == 0){								//firstelement to be enqueued
		tail->pid = newfq->pid;
		tail->fqnext = newfq->fqnext;
		fqueue = tail;
	} else{
		tail->fqnext = newfq;							//attaching new entry to existing queue	
	}
	return pid;
}

int32 f_dequeue (struct fqentry * fqueue, future_mode_t mode)
{
	struct fqentry *head;
	if (mode == FUTURE_SHARED)							//handling shared mode
	{
		while(fqueue != NULL){
			head = fqueue->fqnext;
			fqueue->fqnext = NULL;
			resume (fqueue->pid);						//resuming all dequeued threads
			fqueue = head;
		}
	}
	else if (mode == FUTURE_QUEUE)							//handling queue mode
	{
		if(fqueue->pid >0){
			int resPid = fqueue->pid;
			head = fqueue->fqnext;
			fqueue->pid = head->pid;
			fqueue->fqnext = head->fqnext;
			resume(resPid);							//resuming dequeued thread i.e. first element in queue
		}
	}else{
		return 0;
	}

	return 1;
}

