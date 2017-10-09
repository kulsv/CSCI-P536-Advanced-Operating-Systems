#include <xinu.h>
#include <f_queue.h>
#include <future.h>
pid32 f_enqueue (pid32 pid, struct fqentry* fqueue)
{
	struct fqentry *temp;
	struct fqentry *tail;
	struct fqentry *newfq;
	temp = fqueue;
	tail = fqueue;
	newfq = (struct fqentry*)(getmem (sizeof (struct fqentry)));			//creating new entry in queue
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

pid32 f_dequeue (struct fqentry* fqueue)
{
	pid32 pid;
	struct fqentry *head;
	head = fqueue->fqnext;
	fqueue->fqnext = NULL;
	pid = fqueue->pid;
	fqueue = head;
	return pid;
}

