
resched2.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <resched2>:

struct  defer   Defer;
void    resched2(
        pid32 pid, uint16 next_state
)           /* Assumes interrupts are disabled      */
{
   0:	e92d4070 	push	{r4, r5, r6, lr}
        struct procent *ptold;  /* Ptr to table entry for old process   */
        struct procent *ptnew;  /* Ptr to table entry for new process   */
        struct procent *pt;
        /* If rescheduling is deferred, record attempt and return */

        if (Defer.ndefers > 0) {
   4:	e59f3138 	ldr	r3, [pc, #312]	; 144 <resched2+0x144>
   8:	e5933000 	ldr	r3, [r3]
   c:	e3530000 	cmp	r3, #0
  10:	da000003 	ble	24 <resched2+0x24>
                Defer.attempt = TRUE;
  14:	e3a02001 	mov	r2, #1
  18:	e59f3124 	ldr	r3, [pc, #292]	; 144 <resched2+0x144>
  1c:	e5c32004 	strb	r2, [r3, #4]
                return;
  20:	e8bd8070 	pop	{r4, r5, r6, pc}
        }


        if(next_state < 0 || next_state > 7){
  24:	e3510007 	cmp	r1, #7
  28:	88bd8070 	pophi	{r4, r5, r6, pc}
                return;
        }

        pt = &proctab[pid];
  2c:	e0600180 	rsb	r0, r0, r0, lsl #3
  30:	e1a00180 	lsl	r0, r0, #3
  34:	e59f310c 	ldr	r3, [pc, #268]	; 148 <resched2+0x148>
  38:	e080c003 	add	ip, r0, r3

        if (pt->prstate == PR_CURR) {  /* Process remains eligible */
  3c:	e19020b3 	ldrh	r2, [r0, r3]
  40:	e3520001 	cmp	r2, #1
                /* Old process will no longer remain current */

                pt->prstate = PR_READY;
                insert(currpid, readylist, pt->prprio);
        }else{
              	pt->prstate = next_state;
  44:	118010b3 	strhne	r1, [r0, r3]
                return;
        }

        pt = &proctab[pid];

        if (pt->prstate == PR_CURR) {  /* Process remains eligible */
  48:	1a000012 	bne	98 <resched2+0x98>
                if (pt->prprio > firstkey(readylist)) {
  4c:	e1dc20f2 	ldrsh	r2, [ip, #2]
  50:	e59f10f4 	ldr	r1, [pc, #244]	; 14c <resched2+0x14c>
  54:	e1d110b0 	ldrh	r1, [r1]
  58:	e59fc0f0 	ldr	ip, [pc, #240]	; 150 <resched2+0x150>
  5c:	e6bf4071 	sxth	r4, r1
  60:	e08c4204 	add	r4, ip, r4, lsl #4
  64:	e5944008 	ldr	r4, [r4, #8]
  68:	e5944004 	ldr	r4, [r4, #4]
  6c:	e08cc204 	add	ip, ip, r4, lsl #4
  70:	e59cc008 	ldr	ip, [ip, #8]
  74:	e15c0002 	cmp	ip, r2
  78:	38bd8070 	popcc	{r4, r5, r6, pc}
                        return;
                }

                /* Old process will no longer remain current */

                pt->prstate = PR_READY;
  7c:	e3a0c002 	mov	ip, #2
  80:	e180c0b3 	strh	ip, [r0, r3]
                insert(currpid, readylist, pt->prprio);
  84:	e59f30c8 	ldr	r3, [pc, #200]	; 154 <resched2+0x154>
  88:	e5930000 	ldr	r0, [r3]
  8c:	e6bf1071 	sxth	r1, r1
  90:	ebfffffe 	bl	0 <insert>
  94:	eaffffff 	b	98 <resched2+0x98>
        }


	/* Point to process table entry for the current (old) process */

        ptold = &proctab[currpid];
  98:	e59f30b4 	ldr	r3, [pc, #180]	; 154 <resched2+0x154>
  9c:	e5930000 	ldr	r0, [r3]
  a0:	e060c180 	rsb	ip, r0, r0, lsl #3
  a4:	e1a0c18c 	lsl	ip, ip, #3
  a8:	e59f3098 	ldr	r3, [pc, #152]	; 148 <resched2+0x148>
  ac:	e08c4003 	add	r4, ip, r3

        if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
  b0:	e19c20b3 	ldrh	r2, [ip, r3]
  b4:	e3520001 	cmp	r2, #1
  b8:	1a00000f 	bne	fc <resched2+0xfc>
                if (ptold->prprio > firstkey(readylist)) {
  bc:	e1d420f2 	ldrsh	r2, [r4, #2]
  c0:	e59f1084 	ldr	r1, [pc, #132]	; 14c <resched2+0x14c>
  c4:	e1d110b0 	ldrh	r1, [r1]
  c8:	e59f5080 	ldr	r5, [pc, #128]	; 150 <resched2+0x150>
  cc:	e6bf6071 	sxth	r6, r1
  d0:	e0856206 	add	r6, r5, r6, lsl #4
  d4:	e5966008 	ldr	r6, [r6, #8]
  d8:	e5966004 	ldr	r6, [r6, #4]
  dc:	e0855206 	add	r5, r5, r6, lsl #4
  e0:	e5955008 	ldr	r5, [r5, #8]
  e4:	e1550002 	cmp	r5, r2
  e8:	38bd8070 	popcc	{r4, r5, r6, pc}
                        return;
                }

                /* Old process will no longer remain current */

                ptold->prstate = PR_READY;
  ec:	e3a0e002 	mov	lr, #2
  f0:	e18ce0b3 	strh	lr, [ip, r3]
                insert(currpid, readylist, ptold->prprio);
  f4:	e6bf1071 	sxth	r1, r1
  f8:	ebfffffe 	bl	0 <insert>
        }
	/* Force context switch to highest priority ready process */
  currpid = dequeue(readylist);
  fc:	e59f3048 	ldr	r3, [pc, #72]	; 14c <resched2+0x14c>
 100:	e1d300f0 	ldrsh	r0, [r3]
 104:	ebfffffe 	bl	0 <dequeue>
 108:	e59f3044 	ldr	r3, [pc, #68]	; 154 <resched2+0x154>
 10c:	e5830000 	str	r0, [r3]
        ptnew = &proctab[currpid];
 110:	e0600180 	rsb	r0, r0, r0, lsl #3
 114:	e1a00180 	lsl	r0, r0, #3
 118:	e59f3028 	ldr	r3, [pc, #40]	; 148 <resched2+0x148>
 11c:	e0801003 	add	r1, r0, r3
        ptnew->prstate = PR_CURR;
 120:	e3a02001 	mov	r2, #1
 124:	e18020b3 	strh	r2, [r0, r3]
        preempt = QUANTUM;              /* Reset time slice for process */
 128:	e3a02002 	mov	r2, #2
 12c:	e59f3024 	ldr	r3, [pc, #36]	; 158 <resched2+0x158>
 130:	e5832000 	str	r2, [r3]
#ifdef MMU
	FlushTLB();
        setPageTable();
#endif/*MMU*/

	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
 134:	e2840004 	add	r0, r4, #4
 138:	e2811004 	add	r1, r1, #4
 13c:	ebfffffe 	bl	0 <ctxsw>
 140:	e8bd8070 	pop	{r4, r5, r6, pc}
	...
