		#include <xinu.h>
		#include <kernel.h>
		#include <stddef.h>
		#include <stdio.h>
		#include <string.h>
		
		
		#ifdef FS
		#include <fs.h>
		
		static struct fsystem fsd;
		int dev0_numblocks;
		int dev0_blocksize;
		char *dev0_blocks;
		
		extern int dev0;
		
		char block_cache[512];
		
		#define SB_BLK 0
		#define BM_BLK 1
		#define RT_BLK 2
		
		#define NUM_FD 16
		struct filetable oft[NUM_FD];
		int next_open_fd = 0;
		
		
		#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
		#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
		#define FIRST_INODE_BLOCK 2
		
		int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);
		
		/* YOUR CODE GOES HERE */
		
		
		int fs_create(char *filename, int mode) {
			if(fsd.inodes_used < DEFAULT_NUM_INODES){
			int numentries = fsd.root_dir.numentries;
			int i = 0;
			while(i <= fsd.inodes_used){
				struct filetable fentry = oft[i];
				if((strcmp(filename,fsd.root_dir.entry[numentries].name))==0){
					printf("File name already exists\n");
					return SYSERR;		
				}
				i++;		
			}
			strcpy(fsd.root_dir.entry[numentries].name,filename);
			printf("File Created :: %s\n", fsd.root_dir.entry[numentries].name);
			fsd.root_dir.entry[numentries].inode_num = fsd.inodes_used;
			fsd.root_dir.numentries++;
			struct inode in;
			in.id = fsd.inodes_used;
			in.type = 1;
			in.device = dev0;
			fs_put_inode_by_num(dev0, in.id,&in);  //new change
			fsd.inodes_used++;
			return fsd.inodes_used-1;
		}
		}
		
		int fs_open(char *filename, int flags) {
			if(flags == O_RDWR || flags == O_WRONLY || flags == O_RDONLY){
				int i = 0;
				struct inode *in;
				struct filetable fentry;
				while(i < NUM_FD){
				//struct filetable fentry = oft[i];
					if((strcmp(filename,fsd.root_dir.entry[i].name))==0){			
						fentry.state = 1;
						fentry.in.id = i;
						fentry.in.size = fsd.blocksz;
						fentry.fileptr = 0;
						fentry.flag = flags;
						fs_get_inode_by_num(dev0, i, &(fentry.in));
						oft[i] = fentry;
						return i;		
					}else{
						i++;
					}
				}
			}else{
				printf("File should be in Readonly OR WriteOnly OR Read and Write Mode");					
			}
			return SYSERR;
		}
		
		int fs_close(int fd) {
			int i = 0;
			while(i <=fsd.inodes_used){
				struct filetable fentry = oft[i];
				if(fentry.in.id == fd){
					fentry.state = 0;
					return 1;		
				}else{
					i++;
				}
			}
			return 0;
		}
		
		
		int fs_seek(int fd, int offset) {
			oft[fd].fileptr = offset;		
			return SYSERR;
		}
		
		int fs_read(int fd, void *buf, int nbytes) {
			if(oft[fd].flag == O_RDWR || oft[fd].flag == O_RDONLY){
				int nlen = nbytes;
				int tempsize = 0;
				int blkcount = 0;
				int start = 0;
				int j = 0;
				while(nlen != 0){
						if(nlen > 0 && nlen < fsd.blocksz){
						tempsize = nlen;		
					}else if(nlen > 0 && nlen > fsd.blocksz)
						tempsize = fsd.blocksz;
						int status = bs_bread(dev0, oft[fd].in.blocks[blkcount], 0, buf+start, tempsize);
						start += tempsize;
						nlen = nlen - tempsize;	
						blkcount ++;
				}
			}else{
				printf("File has no read access\n");
				return SYSERR;					
			}
			
			
			return nbytes;
		}
		
		int fs_write(int fd, void *buf, int nbytes) {
			if(oft[fd].flag == O_RDWR || oft[fd].flag == O_WRONLY){
				int i = 15;
				int nblks = 0;
				int nlen = nbytes;
				int count = 0;
				int start = 0;
				char *filename;
				if(nbytes > fsd.blocksz){
					nblks = nbytes/fsd.blocksz;	
					if(nbytes%fsd.blocksz > 0)
						nblks++;	
					}else{
					nblks = 1;
				}
				int j = 0;
					while(i<MDEV_NUM_BLOCKS && nblks > 0) {
				
					if(fs_getmaskbit(i)==0)	{
						 if(nlen > fsd.blocksz){
				 			nlen = nlen - fsd.blocksz;
					 		bs_bwrite(dev0, i, 0, buf+start, fsd.blocksz);
							start += fsd.blocksz;
						}else if(nlen < fsd.blocksz && nlen > 0){
						bs_bwrite(dev0, i, 0, buf+start, nlen);
						nlen = 0;
					}
					fs_setmaskbit(i);
					if(nblks >0){
						oft[fd].in.blocks[count] = i;
						count ++;	
					}
					if(nlen ==0){
						fs_printfreemask();
						oft[fd].in.size = nbytes;
						fs_put_inode_by_num(dev0, oft[fd].in.id,&oft[fd].in);
						return nbytes;
					}
				}
				i++;	
				nblks--;	
			}	
			}else{
				printf("File has no Write access\n");					
			}
			return SYSERR;
		}
		
		int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
		  int diskblock;
		
		  if (fileblock >= INODEBLOCKS - 2) {
		    printf("No indirect block support\n");
		    return SYSERR;
		  }
		
		  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address
		
		  return diskblock;
		}
		
		/* read in an inode and fill in the pointer */
		int
		fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
		  int bl, inn;
		  int inode_off;
		
		  if (dev != 0) {
		    printf("Unsupported device\n");
		    return SYSERR;
		  }
		  if (inode_number > fsd.ninodes) {
		    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
		    return SYSERR;
		  }
		
		  bl = inode_number / INODES_PER_BLOCK;
		  inn = inode_number % INODES_PER_BLOCK;
		  bl += FIRST_INODE_BLOCK;
		
		  inode_off = inn * sizeof(struct inode);
		
		  /*
		  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
		  printf("inn*sizeof(struct inode): %d\n", inode_off);
		  */
		
		  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
		  memcpy(in, &block_cache[inode_off], sizeof(struct inode));
		
		  return OK;
		
		}
		
		int
		fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
		  int bl, inn;
		
		  if (dev != 0) {
		    printf("Unsupported device\n");
		    return SYSERR;
		  }
		  if (inode_number > fsd.ninodes) {
		    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
		    return SYSERR;
		  }
		
		  bl = inode_number / INODES_PER_BLOCK;
		  inn = inode_number % INODES_PER_BLOCK;
		  bl += FIRST_INODE_BLOCK;
		
		  /*
		  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
		  */
		
		  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
		  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
		  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);
		
		  return OK;
		}
		     
		int fs_mkfs(int dev, int num_inodes) {
		  int i;
		  
		  if (dev == 0) {
		    fsd.nblocks = dev0_numblocks;
		    fsd.blocksz = dev0_blocksize;
		  }
		  else {
		    printf("Unsupported device\n");
		    return SYSERR;
		  }
		
		  if (num_inodes < 1) {
		    fsd.ninodes = DEFAULT_NUM_INODES;
		  }
		  else {
		    fsd.ninodes = num_inodes;
		  }
		
		  i = fsd.nblocks;
		  while ( (i % 8) != 0) {i++;}
		  fsd.freemaskbytes = i / 8; 
		  
		  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
		    printf("fs_mkfs memget failed.\n");
		    return SYSERR;
		  }
		  
		  /* zero the free mask */
		  for(i=0;i<fsd.freemaskbytes;i++) {
		    fsd.freemask[i] = '\0';
		  }
		  
		  fsd.inodes_used = 0;
		  
		  /* write the fsystem block to SB_BLK, mark block used */
		  fs_setmaskbit(SB_BLK);
		  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
		  
		  /* write the free block bitmask in BM_BLK, mark block used */
		  fs_setmaskbit(BM_BLK);
		  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);
		
		  return 1;
		}
		
		void
		fs_print_fsd(void) {
		
		  printf("fsd.ninodes: %d\n", fsd.ninodes);
		  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
		  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
		  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
		}
		
		/* specify the block number to be set in the mask */
		int fs_setmaskbit(int b) {
		  int mbyte, mbit;
		  mbyte = b / 8;
		  mbit = b % 8;
		
		  fsd.freemask[mbyte] |= (0x80 >> mbit);
		  return OK;
		}
		
		/* specify the block number to be read in the mask */
		int fs_getmaskbit(int b) {
		  int mbyte, mbit;
		  mbyte = b / 8;
		  mbit = b % 8;
		
		  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
		  return OK;
		
		}
		
		/* specify the block number to be unset in the mask */
		int fs_clearmaskbit(int b) {
		  int mbyte, mbit, invb;
		  mbyte = b / 8;
		  mbit = b % 8;
		
		  invb = ~(0x80 >> mbit);
		  invb &= 0xFF;
		
		  fsd.freemask[mbyte] &= invb;
		  return OK;
		}
		
		/* This is maybe a little overcomplicated since the lowest-numbered
		   block is indicated in the high-order bit.  Shift the byte by j
		   positions to make the match in bit7 (the 8th bit) and then shift
		   that value 7 times to the low-order bit to print.  Yes, it could be
		   the other way...  */
		void fs_printfreemask(void) {
		  int i,j;
		
		  for (i=0; i < fsd.freemaskbytes; i++) {
		    for (j=0; j < 8; j++) {
		      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
		    }
		    if ( (i % 8) == 7) {
		      printf("\n");
		    }
		  }
		  printf("\n");
		}
		
		//insert open here
		
		
		
		
		
		#endif /* FS */
