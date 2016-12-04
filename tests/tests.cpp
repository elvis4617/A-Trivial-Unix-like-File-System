// tests.cpp
#include "../src/FileSystem.cpp"
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

//Sample test cases:

//test create_file for creating with same fileanme

//return code 1 for success
//return code -1 fpr failure
TEST(FSTest, create_file_same_filename_test) {
	myFileSystem f ((char*) "disk0");
	ASSERT_EQ(1,f.create_file((char*)"1.c",8));
	ASSERT_EQ(-1,f.create_file((char*)"1.c",8));
}

//test create_file for maximum number of blocks
//return code 1 for success
//return code -1 fpr failure
TEST(FSTest, create_file_max_blocks) {
	myFileSystem f ((char*) "disk0");
	ASSERT_EQ(-1,f.create_file((char*)"1.c",9));
}

//Test create_file for maximum number of file and maximum disks.
//return code 1 for success
//return code -1 fpr failure
TEST(FSTest, create_file_max_file_and_max_disk) {
	myFileSystem f ((char*) "disk0");
	ASSERT_EQ(1,f.create_file((char*)"2.c",8));
	ASSERT_EQ(1,f.create_file((char*)"3.c",8));
	ASSERT_EQ(1,f.create_file((char*)"4.c",8));
	ASSERT_EQ(1,f.create_file((char*)"5.c",8));
	ASSERT_EQ(1,f.create_file((char*)"6.c",8));
	ASSERT_EQ(1,f.create_file((char*)"7.c",8));
	ASSERT_EQ(1,f.create_file((char*)"8.c",8));
	ASSERT_EQ(1,f.create_file((char*)"9.c",8));
	ASSERT_EQ(1,f.create_file((char*)"10.c",8));
	ASSERT_EQ(1,f.create_file((char*)"11.c",8));
	ASSERT_EQ(1,f.create_file((char*)"12.c",8));
	ASSERT_EQ(1,f.create_file((char*)"13.c",8));
	ASSERT_EQ(1,f.create_file((char*)"14.c",8));
	ASSERT_EQ(1,f.create_file((char*)"15.c",8));
	ASSERT_EQ(-1,f.create_file((char*)"16.c",8));
	ASSERT_EQ(1,f.create_file((char*)"16.c",7));
	ASSERT_EQ(-1,f.create_file((char*)"17.c",8));
}

//test delete_file for filename that is not existed.
//return code 1 for success
//return code -1 for failure
TEST(FSTest, delete_file_not_exist) {
	myFileSystem f ((char*) "disk1");
	int code = f.delete_file((char*)"test.c");
	ASSERT_EQ(-1,code);
}

//test delete_file
//return code 1 for success
//return code -1 for failure
TEST(FSTest, delete_file) {
	myFileSystem f ((char*) "disk1");
	f.create_file((char*)"1.c",5);
	ASSERT_EQ(1,f.delete_file((char*)"1.c"));
}

//test list_file
//return code 1 for success
//return code -1 for failure
TEST(FSTest, list_file) {
	myFileSystem f ((char*) "disk0");
	ASSERT_EQ(1,f.ls());
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
