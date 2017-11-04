	/* This file implements the memory "block store" for use by the in-memory filesystem */
	
	#include <xinu.h>
	#include <stddef.h>
	#include <stdio.h>
	#include <string.h>
	
	extern int dev0_numblocks;
	extern int dev0_blocksize;
	extern char *dev0_blocks;
	int dev0 = 0;
	
	#if FS
	#include <fs.h>
	
	/*Function to create\make device where dev is device number*/
	int bs_mkdev(int dev, int blocksize, int numblocks) {
	
	  if (dev != 0) {
	    printf("Unsupported device: %d\n", dev);
	    return SYSERR;
	  }
	
	  if (blocksize != 0) {
	    dev0_blocksize = blocksize;
	  }
	  else {
	    dev0_blocksize = MDEV_BLOCK_SIZE;
	  }
	
	  if (numblocks != 0) {
	    dev0_numblocks =  numblocks;
	  }
	  else {
	    dev0_numblocks =  MDEV_NUM_BLOCKS;
	  }
	
	  if ( (dev0_blocks = getmem(dev0_numblocks * dev0_blocksize)) == (void *)SYSERR) {
	    printf("mkbsdev memgetfailed\n");
	    return SYSERR;
	  }
	
	  return OK;
	
	}
	
	/*Function to read from block*/
	int bs_bread(int dev, int block, int offset, void *buf, int len) {
	  char *bbase;
	//	printf("inside bs_read..\n");
	  if (dev != 0) {
	    printf("Unsupported device\n");
	    return SYSERR;
	  }
	  if (offset >= dev0_blocksize) {
	    printf("Bad offset\n");
	    return SYSERR;
	  }
	
	  bbase = &dev0_blocks[block * dev0_blocksize];
	//	printf("inside bs_read.. CALLING MEMCPY.. \n");
	  memcpy(buf, (bbase+offset), len);
	
	/*int i = 0;
	   printf("printing characters read ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: \n");
	
	  for(i=0; i<len; i++)
	    {
	       
	        printf("%c", &(buf[i]));
	    }*/
	  return OK;
	
	}
	
	/*Function to write on block*/
	int bs_bwrite(int dev, int block, int offset, void * buf, int len) {
	  char *bbase;
	//  printf("lennnn :: %d\n", len);
	  int i = 0;
	 // printf("printing characters to be written ::::::::::::::::: \n");
	
	 /*for(i=0; i<len; i++)
	    {
	       
	        printf("%c", &(buf[i]));
	    }*/
		//printf("inside bs_write..\n");
	  if (dev != 0) {
	    printf("Unsupported device\n");
	    return SYSERR;
	  }
	  if (offset >= dev0_blocksize) {
	    printf("Bad offset\n");
	    return SYSERR;
	  }
	
	  bbase = &dev0_blocks[block * dev0_blocksize];
	//printf("inside bs_write.. CALLING MEMCPY.. \n");
	//printf("bbase :: %c\n", bbase);
	  memcpy((bbase+offset), buf, len);
	  
	  return OK;
	
	}
	
	#endif /* FS */
