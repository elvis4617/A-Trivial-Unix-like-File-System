gcc create_fs.c -o create_fs
./create_fs disk0
./create_fs disk1

make

./runTests

rm disk0 disk1 create_fs

make clean