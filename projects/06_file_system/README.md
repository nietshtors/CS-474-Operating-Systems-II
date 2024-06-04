# Project 6: Continuing the File System, managing free blocks 

## Building

Command line:

* `make` to build. An executable called `testfs` will be produced.
* `make clean` to clean up all build products except the executable.
* `make pristine` to clean up all build products entirely.


VS Code:

* The default build task runs `make`.

## Files

* `testfs.c`: The main code
* `image.c`: Code to open and close a file
* `block.c`: Code for reading and writing blocks
* `free.c`: Code for finding free bits and setting bits
* `inode.c`: Code for allocating inodes
* `ctest.h`: Testing suite

## Functions

* `main()`
  * `image_open`: Opens a file with the option to truncate
  * `image_close()`: Closes the last opened file
  * `bread()`: Reads a block into the given buffer
  * `bwrite()`: Writes a block from the given buffer