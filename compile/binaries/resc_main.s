
resched.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <resched>:
/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
   0:	e92d4070 	push	{r4, r5, r6, lr}
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
   4:	e59f30c4 	ldr	r3, [pc, #196]	; d0 <resched+0xd0>
   8:	e5933000 	ldr	r3, [r3]
   c:	e3530000 	cmp	r3, #0
  10:	da000003 	ble	24 <resched+0x24>
		Defer.attempt = TRUE;
  14:	e3a02001 	mov	r2, #1
  18:	e59f30b0 	ldr	r3, [pc, #176]	; d0 <resched+0xd0>
  1c:	e5c32004 	strb	r2, [r3, #4]
		return;
  20:	e8bd8070 	pop	{r4, r5, r6, pc}
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];
  24:	e59f30a8 	ldr	r3, [pc, #168]	; d4 <resched+0xd4>
  28:	e5930000 	ldr	r0, [r3]
  2c:	e060c180 	rsb	ip, r0, r0, lsl #3
  30:	e1a0c18c 	lsl	ip, ip, #3
  34:	e59f309c 	ldr	r3, [pc, #156]	; d8 <resched+0xd8>
  38:	e08c4003 	add	r4, ip, r3

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
  3c:	e19c20b3 	ldrh	r2, [ip, r3]
  40:	e3520001 	cmp	r2, #1
  44:	1a00000f 	bne	88 <resched+0x88>
		if (ptold->prprio > firstkey(readylist)) {
  48:	e1d420f2 	ldrsh	r2, [r4, #2]
  4c:	e59f1088 	ldr	r1, [pc, #136]	; dc <resched+0xdc>
  50:	e1d110b0 	ldrh	r1, [r1]
  54:	e59f5084 	ldr	r5, [pc, #132]	; e0 <resched+0xe0>
  58:	e6bf6071 	sxth	r6, r1
  5c:	e0856206 	add	r6, r5, r6, lsl #4
  60:	e5966008 	ldr	r6, [r6, #8]
  64:	e5966004 	ldr	r6, [r6, #4]
  68:	e0855206 	add	r5, r5, r6, lsl #4
  6c:	e5955008 	ldr	r5, [r5, #8]
  70:	e1550002 	cmp	r5, r2
  74:	38bd8070 	popcc	{r4, r5, r6, pc}
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
  78:	e3a0e002 	mov	lr, #2
  7c:	e18ce0b3 	strh	lr, [ip, r3]
		insert(currpid, readylist, ptold->prprio);
  80:	e6bf1071 	sxth	r1, r1
  84:	ebfffffe 	bl	0 <insert>
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
  88:	e59f304c 	ldr	r3, [pc, #76]	; dc <resched+0xdc>
  8c:	e1d300f0 	ldrsh	r0, [r3]
  90:	ebfffffe 	bl	0 <dequeue>
  94:	e59f3038 	ldr	r3, [pc, #56]	; d4 <resched+0xd4>
  98:	e5830000 	str	r0, [r3]
	ptnew = &proctab[currpid];
  9c:	e0600180 	rsb	r0, r0, r0, lsl #3
  a0:	e1a00180 	lsl	r0, r0, #3
  a4:	e59f302c 	ldr	r3, [pc, #44]	; d8 <resched+0xd8>
  a8:	e0801003 	add	r1, r0, r3
	ptnew->prstate = PR_CURR;
  ac:	e3a02001 	mov	r2, #1
  b0:	e18020b3 	strh	r2, [r0, r3]
	preempt = QUANTUM;		/* Reset time slice for process	*/
  b4:	e3a02002 	mov	r2, #2
  b8:	e59f3024 	ldr	r3, [pc, #36]	; e4 <resched+0xe4>
  bc:	e5832000 	str	r2, [r3]
#ifdef MMU
	FlushTLB();
	setPageTable();
#endif/*MMU*/

	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
  c0:	e2840004 	add	r0, r4, #4
  c4:	e2811004 	add	r1, r1, #4
  c8:	ebfffffe 	bl	0 <ctxsw>
  cc:	e8bd8070 	pop	{r4, r5, r6, pc}
	...

000000e8 <resched_cntl>:
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
  e8:	e92d4008 	push	{r3, lr}
	switch (defer) {
  ec:	e3500001 	cmp	r0, #1
  f0:	0a000002 	beq	100 <resched_cntl+0x18>
  f4:	e3500002 	cmp	r0, #2
  f8:	0a00000b 	beq	12c <resched_cntl+0x44>
  fc:	ea000019 	b	168 <resched_cntl+0x80>

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
 100:	e59f2088 	ldr	r2, [pc, #136]	; 190 <resched_cntl+0xa8>
 104:	e5923000 	ldr	r3, [r2]
 108:	e2831001 	add	r1, r3, #1
 10c:	e5821000 	str	r1, [r2]
 110:	e3530000 	cmp	r3, #0
 114:	1a000015 	bne	170 <resched_cntl+0x88>
			Defer.attempt = FALSE;
 118:	e3a02000 	mov	r2, #0
 11c:	e59f306c 	ldr	r3, [pc, #108]	; 190 <resched_cntl+0xa8>
 120:	e5c32004 	strb	r2, [r3, #4]
		}
		return OK;
 124:	e3a00001 	mov	r0, #1
 128:	e8bd8008 	pop	{r3, pc}

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
 12c:	e59f305c 	ldr	r3, [pc, #92]	; 190 <resched_cntl+0xa8>
 130:	e5933000 	ldr	r3, [r3]
 134:	e3530000 	cmp	r3, #0
 138:	da00000e 	ble	178 <resched_cntl+0x90>
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
 13c:	e2433001 	sub	r3, r3, #1
 140:	e59f2048 	ldr	r2, [pc, #72]	; 190 <resched_cntl+0xa8>
 144:	e5823000 	str	r3, [r2]
 148:	e3530000 	cmp	r3, #0
 14c:	1a00000b 	bne	180 <resched_cntl+0x98>
 150:	e5d23004 	ldrb	r3, [r2, #4]
 154:	e3530000 	cmp	r3, #0
 158:	0a00000a 	beq	188 <resched_cntl+0xa0>
			resched();
 15c:	ebfffffe 	bl	0 <resched>
		}
		return OK;
 160:	e3a00001 	mov	r0, #1
 164:	e8bd8008 	pop	{r3, pc}

	    default:
		return SYSERR;
 168:	e3e00000 	mvn	r0, #0
 16c:	e8bd8008 	pop	{r3, pc}
	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;
 170:	e3a00001 	mov	r0, #1
 174:	e8bd8008 	pop	{r3, pc}

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
 178:	e3e00000 	mvn	r0, #0
 17c:	e8bd8008 	pop	{r3, pc}
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;
 180:	e3a00001 	mov	r0, #1
 184:	e8bd8008 	pop	{r3, pc}
 188:	e3a00001 	mov	r0, #1

	    default:
		return SYSERR;
	}
}
 18c:	e8bd8008 	pop	{r3, pc}
 190:	00000000 	.word	0x00000000
