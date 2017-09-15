#include <xinu.h>
#include <string.h>
#include <stdio.h>

process polling_proc(int32 *intArray1, int32 *intArray2, int32 *pCurr, int32 *rCurr);
process msg_Passing(pid32 main_pid, pid32 parent_pid, int32 *next_pid, int32 *rCurr, int32 pCurr);
int32 rCount;

