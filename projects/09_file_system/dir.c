#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dir.h"
#include "block.h"
#include "inode.h"
#include "pack.h"
#include "dirbasename.c"


void mkfs(void)
{
    struct inode *in = ialloc();
    alloc();  // Superblock
    alloc();  // inode map
    alloc();  // Block map
    alloc();  // inode data block 0
    alloc();  // inode data block 1
    alloc();  // inode data block 2
    alloc();  // inode data block 3
    int data_block_num = alloc();

    in->flags = FLAGS_DIRECTORY;
    in->size = 2 * DIRECTORY_SIZE;
    in->block_ptr[0] = data_block_num;

    unsigned char block[BLOCK_SIZE];
    write_u16(block, in->inode_num);
    strcpy((char *)(block + 2), ".");
    write_u16(block + DIRECTORY_SIZE, in->inode_num);
    strcpy((char *)(block + DIRECTORY_SIZE + 2), "..");
    
    bwrite(data_block_num, block);
    iput(in);
}

/* We pass in an inode number and it should give us a struct back
 * representing the open directory, or NULL if there was an error. */
struct directory *directory_open(int inode_num)
{
    struct inode *in = iget(inode_num);
    if (in == NULL)
        return NULL;
    struct directory *dir = malloc(sizeof(struct directory));
    dir->inode = in;
    dir->offset = 0;
    return dir;
}

/* This function takes a struct directory * obtained from directory_open(),
 * and it takes a pointer to a struct directory_entry that it will fill out
 * with the entry information. It returns 0 on success, and -1 on failure. */
int directory_get(struct directory *dir, struct directory_entry *ent)
{
    if (dir->offset >= dir->inode->size)
        return -1;
    int data_block_index = dir->offset / BLOCK_SIZE;
    int data_block_num = dir->inode->block_ptr[data_block_index];

    unsigned char block[BLOCK_SIZE];
    bread(data_block_num, block);

    int offset_in_block = dir->offset % BLOCK_SIZE;
    ent->inode_num = read_u16(block + offset_in_block);
    strcpy(ent->name, (char *)(block + offset_in_block + 2));
    dir->offset += DIRECTORY_SIZE;
    return 0;
}

/* Closes a directory */
void directory_close(struct directory *d)
{
    iput(d->inode);
    free(d);
}

/* Map a file specified by a path to that file's in-core inode.
 * Returns NULL on failure. */
struct inode *namei(char *path)
{
    struct inode *in;
    if (!strcmp(path, "/"))
        in = iget(ROOT_INODE_NUM);
    else
        in = NULL;
    return in;
}

/* Creates a directory. It returns 0 on success, and -1 on failure. */
int directory_make(char *path)
{
    char dirname[1024];
    get_dirname(path, dirname);

    char basename[16];
    get_basename(path, basename);

    struct inode *in_parent = namei(dirname);
    if (in_parent == NULL)
        return -1;

    struct inode *in = ialloc();
    if (in == NULL)
        return -1;

    int data_block_num = alloc();
    if (data_block_num == -1)
        return -1;

    unsigned char block[BLOCK_SIZE];
    write_u16(block, in->inode_num);
    strcpy((char *)(block + 2), ".");
    write_u16(block + DIRECTORY_SIZE, in_parent->inode_num);
    strcpy((char *)(block + DIRECTORY_SIZE + 2), "..");
    
    in->flags = FLAGS_DIRECTORY;
    in->size = 2 * DIRECTORY_SIZE;
    in->block_ptr[0] = data_block_num;

    bwrite(data_block_num, block);

    int block_index = in_parent->size / BLOCK_SIZE;
    data_block_num = in_parent->block_ptr[block_index];

    bread(data_block_num, block);
    write_u16(block + in_parent->size, in->inode_num);
    strcpy((char *)(block + in_parent->size + 2), basename);

    bwrite(data_block_num, block);

    in_parent->size += DIRECTORY_SIZE;

    iput(in);
    iput(in_parent);

    return 0;
}
