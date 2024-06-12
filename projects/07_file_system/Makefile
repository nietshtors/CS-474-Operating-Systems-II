CC=gcc
CCOPTS=-Wall -Wextra -Werror
LIBS=-DCTEST_ENABLE

testfs: libvvsfs.a
	$(CC) -o $@ $^

libvvsfs.a: testfs.o image.o block.o free.o inode.o pack.o
	ar rcs $@ $^

testfs.o: testfs.c image.h block.h ctest.h
	$(CC) $(CCOPTS) -c -o $@ $< $(LIBS)

image.o: image.c image.h
	$(CC) $(CCOPTS) -c -o $@ $< $(LIBS)

block.o: block.c block.h
	$(CC) $(CCOPTS) -c -o $@ $< $(LIBS)

free.o: free.c free.h
	$(CC) $(CCOPTS) -c -o $@ $< $(LIBS)

inode.o: inode.c inode.h
	$(CC) $(CCOPTS) -c -o $@ $< $(LIBS)

pack.o: pack.c pack.h
	$(CC) $(CCOPTS) -c -o $@ $< $(LIBS)

.PHONY: test clean pristine

test: testfs

clean:
	rm -f $(wildcard *.o)

pristine: clean
	rm -f testfs $(wildcard *.a)