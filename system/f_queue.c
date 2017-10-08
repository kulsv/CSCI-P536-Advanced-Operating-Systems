#include <xinu.h>
#include <f_queue.h>
#include <future.h>
pid32
f_enqueue (pid32 pid, struct fqentry *fqueue)
{
  	struct fqentry *temp;
  	struct fqentry *tail;
  	struct fqentry *newfq;
 	 temp = (struct qentry*)(getmem (sizeof (struct fqentry)));
 	 temp = fqueue;
 	 tail = (struct qentry*)(getmem (sizeof (struct fqentry)));
 	 tail = fqueue;
 	 newfq = (struct qentry*)(getmem (sizeof (struct fqentry)));
 	 newfq->pid = pid;
  	 newfq->fqnext = NULL;
  	 while (temp != NULL)
    {
      if (temp->fqnext == NULL)
		{
	 		 tail = temp;
	  		 break;
		}
      else
		{
			  temp = temp->fqnext;
		}
    }
    
  		if (tail->pid == 0){
  			 tail->pid = newfq->pid;
  			 tail->fqnext = newfq->fqnext;
  			 fqueue = tail;
 		} else{
   		 tail->fqnext = newfq;
   	}
  
  	return pid;
}

int32 f_dequeue (struct fqentry * fqueue, future_mode_t mode)
{
  struct fqentry *head;
		if (mode == FUTURE_SHARED)
    {
     
      while(fqueue != NULL){
      	head = fqueue->fqnext;
      	fqueue->fqnext = NULL;
    		resume (fqueue->pid);
      	 fqueue = head;
     	 	
     }
    }
  else if (mode == FUTURE_QUEUE)
    {
     		if(fqueue->pid >0){
     			int resPid = fqueue->pid;
           	head = fqueue->fqnext;
         	fqueue->pid = head->pid;
         	fqueue->fqnext = head->fqnext;
         	resume(resPid);
         }
         	
      
      	
    }else{
		return 0;    
    }

  return 1;
}
