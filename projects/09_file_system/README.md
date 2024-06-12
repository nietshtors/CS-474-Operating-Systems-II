# Project 9: Continuing the File System, mkdir and namei

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
* `dir.c`: Code for manipulating directories
* `ctest.h`: Testing suite

## Functions

* `main()`
  * `image_open()`: Opens a file with the option to truncate
  * `mkfs()`: Creates the root directory of the file system
    * `ialloc()`: Allocates a free inode
      * `bread()`: Reads a block into the given buffer
      * `set_free()`: Sets a specific bit value in a block
      * `write_inode()`: Writes an inode to disk
    * `alloc()`: Allocates a free data block
      * `find_free()`: Returns the earliest free bit
        * `find_low_clear_bit()`: Helper function to find the lowest clear bit in a byte
    * `bwrite()`: Writes a block from the given buffer
    * `iput()`: Writes an inode to disk
  * `ls()`: Lists directory contents
    * `directory_open()`: Opens a directory
      * `iget()`: Returns an incore inode
        * `incore_find()`: Finds a specific inode
        * `incore_find_free()`: Finds the first free inode in the incore array
        * `read_inode()`: Reads an inode into a buffer
    * `directory_get()`: Iterates through a directory returning individual entries
    * `directory_close()`: Closes a directory
  * `image_close()`: Closes the last opened file