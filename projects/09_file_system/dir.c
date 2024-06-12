#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dir.h"
#include "block.h"
#include "inode.h"
#include "pack.h"

void mkfs(void)
{
    struct inode *in = ialloc();
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
    int data_block_index = dir->offset / 4096;
    int data_block_num = dir->inode->block_ptr[data_block_index];
    unsigned char block[BLOCK_SIZE];
    bread(data_block_num, block);

    int offset_in_block = dir->offset % 4096;
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