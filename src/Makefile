CC=gcc
CXX=g++

all: create_fs fs

create_fs: create_fs.o
	$(CC) create_fs.o -o create_fs

create_fs.o: create_fs.c
	$(CC) -c create_fs.c

fs: fs.o
	$(CXX) main.o -o fs

fs.o: main.cpp
	$(CXX) -c main.cpp

clean:
	rm -f *~ *.o
	rm -f create_fs fs