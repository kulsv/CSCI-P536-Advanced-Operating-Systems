Assignment 6

Description of the implementation of the functions used in file system is as follows:

(i) int fs_create(char *filename, int mode);
	This function basically creates a new file or directory and saves its name, updates its entry in directory entry and calls the function fs_put_inode_by_num() and saves the new entry for the new inode and 		its respective values.
	If file with the same file name already exists, it throws and error.

(ii) int fs_open(char *filename, int flags):
	This function returns the value of File Descriptor.
	It first checks if the file is requested to open in which mode i.e. Read, Write or Read and Write, both. It then makes a new entry in a Open File Table structure with the corresponding values and assigns 		it default memory of 1 block.
	It then changes the status of the file to open i.e. 1 in filetable structure.
	If any file that is not created and is tried to open, the appropriate error is displayed.
	If file does not have proper flag to be set, appropriate error message is displayed.

(iii) int fs_close(int fd):
	This function accepts the value of File descriptor in its parameter. It then locates the particular file in OFT table, changes its state to 0 i.e. closed.

(iv)  int fs_write(int fd, void *buf, int nbytes):
	This function writes the data from buffer into the memory block.
	It first checks the flag of the file requested to be written. If it has read access, then only it continues to execute else it returns an error.
	So, the process starts from the number pf blocks required to write the whole data. Thus, the vlaue of number of blocks required is calculated. Data in buffer is divided in memory chunks of 512 Bytes as 1 		block consists of 512 bytes. IF more data is present in the block, additional block is provided to that file. 
	For writing data into block, first, free block is located, then bs_bwrite() is called to write the data from buffer to memory block. After this, the bit mask is set as 1  as that block is used. ANd next 		data from buffer is read to write the next chunk of data if present.
	Appropriate vlaue of that block is stored in the corresponding inode-blocks[].

(v)  int fs_read(int fd, void *buf, int nbytes):
	This function reads the data from the memory block and displays it.
	It first checks the flag of the file requested to be read. If it has read access, then only it continues to execute else it returns an error.
	After this check, the function locates the number of memory blocks and their locations for that particular file.
	ONce memory blocks are located, it then calls bs_bread() function to read the mentioned number of bytes from the given memory block and updates the number of bytes to be read from the next byte.
	This function runs until end of all memory blocks is reached.

(vi)  int fs_seek(int fd, int offset):
	This function basically updates and sets the value of file-pointer for the file it is called.
	This file pointer is used to read the data from the mentioned position.


Lessons Learned : 
(i) First design the structure properly.
(ii) Write a pseudocode that follows the structure.
(iii) Write the report while designing the code as more ideas and minor changes strike while writing a report.
