// tests.cpp
#include "../src/FileSystem.cpp"
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

//Sample test cases:

//test create_file return code 1 for success
TEST(FSTest, create_file_test) {
	myFileSystem f ((char*) "disk0");
	int code = f.create_file((char*)"test.c",1);
	ASSERT_EQ(1,code);
}

//test delete_file return code -1 for failure
TEST(FSTest, delete_file_test) {
	myFileSystem f ((char*) "disk1");
	int code = f.delete_file((char*)"test.c");
	ASSERT_EQ(-1,code);
}

//TODO: add test cases here

// TEST(test_case_name, test_name) {
//  ... test body ...
// }


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
