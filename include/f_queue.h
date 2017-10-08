
#ifndef _FQUEUE_H_
#define _FQUEUE_H_  
struct fqentry{		/* One per process plus two per list	*/
	pid32 pid;
	struct fqentry* fqnext;		/* Index of next process or tail	*/
};


#endif /* _FQUEUE_H_ */