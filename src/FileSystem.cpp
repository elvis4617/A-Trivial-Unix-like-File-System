#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

//block size = 1KB
#define block_size 1024

//structure for the inode
struct idxNode
{
  char name[8]; //file name
  int size;     // file size (in number of blocks)
  int blockPointers[8]; // direct block pointers
  int used;             // 0 => inode is free; 1 => in use
};

class myFileSystem
{
private:
  fstream disk;
public:


  myFileSystem(char diskName[16])
  {
   // open the file with the above name
   // this file will act as the "disk" for your file system
   disk.open(diskName);
  }

  int create_file(char name[8], int size){
  if(size > 8) return -1;

  //create a file with this name and this size
  // high level pseudo code for creating a new file

  // Step 1: check to see if we have sufficient free space on disk by
  // reading in the free block list. To do this:
  // move the file pointer to the start of the disk file.
  // Read the first 128 bytes (the free/in-use block information)
  // Scan the list to make sure you have sufficient free blocks to
  // allocate a new file of this size
  disk.seekg(0, ios::beg);
  char* buff = new char[128];
  if(disk.is_open())
    disk.read(buff, 128);
  else
    return -1;
  int freeSpace = 0;
  for(int i = 0; i<128; i++){
    if(buff[i] == 0) freeSpace++;
    if(freeSpace > size) break;
  }
  if(freeSpace < size) return -1;

  // Step 2: we look  for a free inode om disk
  // Read in a inode
  // check the "used" field to see if it is free
  // If not, repeat the above two steps until you find a free inode
  // Set the "used" field to 1
  // Copy the filename to the "name" field
  // Copy the file size (in units of blocks) to the "size" field

  int inodeIdx;
  char* inodeBuff = new char[48];
  idxNode* inode = NULL;
  for(int i = 0; i<16;i++){
    disk.read(inodeBuff, 48);
    idxNode* tempNode = (idxNode*) inodeBuff;
    if(tempNode -> used == 0 && inode == NULL){
      inode = tempNode;
      inodeIdx = i;
    }else if(tempNode -> used && !strcmp(tempNode -> name, name))
      return -1;
  }

  if(inode == NULL) return -1;

  // Step 3: Allocate data blocks to the file
  // for(i=0;i<size;i++)
    // Scan the block list that you read in Step 1 for a free block
    // Once you find a free block, mark it as in-use (Set it to 1)
    // Set the blockPointer[i] field in the inode to this block number.
    //
  // end for

  int count = 0;
  for(int i = 0; i < 128 && count < size; i++){
    if(buff[i] == 0) {
      buff[i] = 1;
      for(int j = 0; j< 8; j++){
        if(inode -> blockPointers[j] == 0){
          inode -> blockPointers[j] = i;
          break;
        }
      }
      count++;
    }
  }
  strcpy(inode -> name, name);
  inode -> size = size;
  inode -> used = 1;

  // Step 4: Write out the inode and free block list to disk
  //  Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode
  disk.seekp(0, ios::beg);
  disk.write(buff, 128);
  disk.seekp(128+48*inodeIdx, ios::beg);
  disk.write((char*) inode, 48);

  return 1;
  } // End Create



  int delete_file(char name[8])
  {
    // Delete the file with this name

    // Step 1: Locate the inode for this file
    // Move the file pointer to the 1st inode (129th byte)
    // Read in an inode
    // If the inode is free, repeat above step.
    // If the inode is in use, check if the "name" field in the
    //   inode matches the file we want to delete. If not, read the next
    //   inode and repeat
    disk.seekg(128, ios::beg);
    char* inodeBuff = new char[48];
    idxNode* inode = NULL;
    int inodeIdx;
    for(int i = 0; i<16; i++){
      disk.read(inodeBuff, 48);
      idxNode* tempNode = (idxNode*) inodeBuff;
      if(tempNode -> used == 1 && !strcmp(tempNode -> name, name)){
        inode = tempNode;
        inodeIdx = i;
        break;
      }
    }
    if(inode == NULL) return -1;

    // Step 2: free blocks of the file being deleted
    // Read in the 128 byte free block list (move file pointer to start
    //   of the disk and read in 128 bytes)
    // Free each block listed in the blockPointer fields as follows:
    // for(i=0;i< inode.size; i++)
      // freeblockList[ inode.blockPointer[i] ] = 0;

      disk.seekg(0, ios::beg);
      char* buff = new char[128];
      if(disk.is_open())
        disk.read(buff, 128);
      else
        return -1;
      for(int i = 0; i < inode -> size; i++){
        if(inode->blockPointers[i] != 0)
          buff[inode->blockPointers[i]] = 0;
      }


    // Step 3: mark inode as free
    // Set the "used" field to 0.
    inode -> used = 0;

    // Step 4: Write out the inode and free block list to disk
    // Move the file pointer to the start of the file
    // Write out the 128 byte free block list
    // Move the file pointer to the position on disk where this inode was stored
    // Write out the inode
    disk.seekp(0, ios::beg);
    disk.write(buff, 128);
    disk.seekp(128+48*inodeIdx, ios::beg);
    disk.write((char*) inode, 48);


    return 1;
  } // End Delete


  int ls(void)
  {
  // List names of all files on disk

  // Step 1: read in each inode and print!
  // Move file pointer to the position of the 1st inode (129th byte)
  // for(i=0;i<16;i++)
    // REad in a inode
    // If the inode is in-use
      // print to list.txt the "name" and "size" fields from the inode
  // end for
  disk.seekg(128, ios::beg);
  ofstream o("list.txt");
  char* inodeBuff = new char[48];
  for(int i = 0; i<16; i++){
    disk.read(inodeBuff, 48);
    idxNode* inode = (idxNode*) inodeBuff;
    if(inode -> used == 1){
      o << "file name: "<< inode -> name << " size: " << inode -> size << endl;
    }
  }
  o.close();
  return 1;

  } // End ls

  int read(char name[8], int blockNum, char buf[1024])
  {

   // read this block from this file

    // Step 1: locate the inode for this file
    // Move file pointer to the position of the 1st inode (129th byte)
    // Read in an inode
    // If the inode is in use, compare the "name" field with the above file
    // If the file names don't match, repeat
  	disk.seekg(128, ios::beg);
  	char* inodeBuff = new char[48];
  	idxNode* inode = NULL;
  	for(int i = 0; i<16; i++){
    	disk.read(inodeBuff, 48);
    	idxNode* tempinode = (idxNode*) inodeBuff;
    if(tempinode->used == 1 && !strcmp(tempinode->name, name)){
      	inode = tempinode;
      	break;
    	}
  	}
  	if(inode == NULL) return -1;

    // Step 2: Read in the specified block
    // Check that blockNum < inode.size, else flag an error
    // Get the disk address of the specified block
    // That is, addr = inode.blockPointer[blockNum]
    // Move the file pointer to the block location (i.e., to byte #
    //   addr*1024 in the file)
  	if(blockNum < inode->size){
  		int addr = inode->blockPointers[blockNum];
  		disk.seekg(addr*1024, ios::beg);
  	}
  	else{
  		return -1;
  	}
    // Read in the block => Read in 1024 bytes from this location
    //   into the buffer "buf"
    disk.read(buf, 1024);
    return 1;
  } // End read


  int write(char name[8], int blockNum, char buf[1024])
  {

   // write this block to this file

    // Step 1: locate the inode for this file
    // Move file pointer to the position of the 1st inode (129th byte)
    // Read in a inode
    // If the inode is in use, compare the "name" field with the above file
    // If the file names don't match, repeat
    disk.seekg(128, ios::beg);
    char* inodeBuff = new char[48];
    idxNode* inode = NULL;
    for(int i = 0; i<16; i++){
      disk.read(inodeBuff, 48);
      idxNode* tempNode = (idxNode*) inodeBuff;
      if(tempNode -> used == 1 && !strcmp(tempNode -> name, name)){
        inode = tempNode;
        break;
      }
    }
    if(inode == NULL) return -1;

    // Step 2: Write to the specified block
    // Check that blockNum < inode.size, else flag an error
    // Get the disk address of the specified block
    // That is, addr = inode.blockPointer[blockNum]
    // Move the file pointer to the block location (i.e., byte # addr*1024)

    // Write the block! => Write 1024 bytes from the buffer "buff" to
    //   this location
    if(blockNum > inode -> size) return -1;
    int diskAddr = inode -> blockPointers[blockNum];
    disk.seekp(1024*diskAddr, ios::beg);

    disk.write(buf, 1024);

    return 1;
  } // end write

  int close_disk()
  {
    // close the file(disk) opened in the constructor
    // this is to check the persistency of file system
    disk.close();
    return 0;
  }

};
