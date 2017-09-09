#include <xinu.h>
#include <string.h>
#include <stdio.h>

process polling_proc(int32 *intArray1, int32 *intArray2, int32 *pCurr, int32 *rCurr);
process msg_Pass(pid32 parent_pid);


int32 pCurr = 1;
int32 rCurr = 1;
int32 processArray[];
int32 parentProcess;
int32 currProcess;
int32 nextProcess;
int32 pCount;
int32 rCount;
int32 totalCount;
int32 firstFlag = 1;
int32 printCount;
int32 count = 0;





