# Once you have written your version of the file system, write a design document that documents your design choices here.

# Describe whether this assignment is individual or group work. List group member names and email addresses here.
This assignment is done in a group.
Group members are:
Elvis Chen (chongzechen@umass.edu)
Minxin Gao(mgao@umass.edu)

#  Describe your design and design decisions here
For the constructor of FileSystem.cpp, we used fstream's method open to open the diskName which is passed in as a parameter.

For the create_file method, we first check if the size is over 8. if it is, we return error code -1. we then set the file pointer
to the start of the disk file using seekg() method of fstream(found on http://www.cplusplus.com/reference/fstream/fstream/)
We checked if the disk is open by calling is_open() else we return an error code -1. Then we read the first 128 bytes and
used a for loop that loops through the entire 128 bytes to check whether they're free. If there are more freespace than the
size required for the file then we break out of the for loop to not waste any time. If the  size of the to-be-created file is
larger than the freespace counts, then we return error code -1. We then look for inode that is not in used. For that we used a
for loop that loops through every inode and look to see if their used value is 0 by using a buffer list. If it is,  then we set
a variable to that inode. If the inode is never set to anything(still NULL) we return error code -1. We then used a for loop using
variable i through the buffer list to check if the block is free. If it is, we set it to 1. We used a for loop using j that goes
from 0 to 7. We then set the blockPointers[j] of the inode to i if it is 0 and break out of the loop.  We then set inode's name,
size to name of the file to be created, and change the used to 1. At last we write out the inode and free block list to disk.
To do this we move the file pointer to the start of the file using seekp(). Then write the 128 byte of the buffer list of free
block list. Then we moved the file pointer to the postion on disk where this inode was tored using seekp() to 128 + 48 * inodeIdx.
This is because inode is after 128 bytes and each inode is 48 bytes big this then is multiplied by the index of where the inode is.
Then we write out the inode using write(). We then return 1 to indicate file created successfully.

For delete_file() we first locate the inode for this. To do this we first move the file pointer to the 1st inode using seekg().
Then like from create_file, we read in all the inode using for loop. We check the condition of if inode is in use, and we use strcmp
to compare the name to see if they're the same. We return -1 if the file is not found. Next we free the blocks of the file being deleted.
To do this we first move the pointer to the start of the disk using seekg() and read in the 128 bytes free block list using read().
We check to see if disk is open before we call read(), if not we return -1. Then using a for loop from 0 to inode's size and free
each block listed in the blockPointer of the inode by setting them to 0. Then then set the used field to 0. We then write out the
inode and free block list to disk just like create_file(). We then return 1 to indicate file was deleted successfully.

For ls() we move the file pointer to the postition of the 1st inode using seekg(). Then we create an ofstream o to open list.txt.
Just like before we loop through each inode and output their name and size to the list.txt if it's used field is 1.
Then we close the ofstream and return 1 to indicate success.

For read() and write() we first need to locate the inode for this file. This is done the same way as delete_file().
We then need to read/write in the specific block. To do this we first check if blockNum is less than inode.size to make sure we don't exceed inode's size.
If it does then we return -1 to indicate failure. We then get the disk address of the specified block by getting the int addr = blockPointers[blockNum].
We then move the file pointer to the block location which is addr*1024. We then use read/write into the buf from the paramter and return 1 to indicate success.


# Briefly describe you test cases and testing efforts here
create_file:
      1. test for the same file name
      2. test for the maximum blocks
      3. test for maximum files and maximum disk space

delete_file:
      1. test for deleting unexisted file.
      2. test for normal delete flow.

ls:
      1. test for ls all files in disk0.

write:
      1. test for writing to not existed file.
      2. test for normal writing flow.

read:
      1. test for reading not existed file.
      2. test for normal reading flow.(read the data that write to block in write test.)

These basically covers all tests that are needed for this system. Especially combining write and read test, we are able to test a completed flow of writing and reading.



# Briefly describe your debugging efforts here
For debugging we have used printf statements inside different methods to print to the console different variables such as inode's name, size, blocks etc.
This is also done with gdb in which p statement is used to p what's contained in a variable and how the values changes each step. We also wrote the test cases prior to the actual coding,
so that we can run tests during the actual coding process.
