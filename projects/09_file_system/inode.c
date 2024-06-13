#include <stddef.h>
#include <stdio.h>
#include "inode.h"
#include "block.h"
#include "free.h"
#include "pack.h"

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

/* Allocate a previously-free inode in the inode map.
 * Returns NULL on failure. */
struct inode *ialloc(void)
{
    unsigned char block[BLOCK_SIZE];
    bread(INODE_MAP, block);
    int inode_num = find_free(block);
    if (inode_num == -1)
        return NULL;
    struct inode *in = iget(inode_num);
    if (in == NULL)
        return NULL;
    set_free(block, inode_num, 1);
    in->size = 0;
    in->owner_id = 0;
    in->permissions = 0;
    in->flags = 0;
    for (int i = 0; i < INODE_PTR_COUNT; i++)
        in->block_ptr[i] = 0;
    in->inode_num = inode_num;

    write_inode(in);
    bwrite(INODE_MAP, block);

    return in;
}

/* This finds the first free in-core inode in the incore
 * array. It returns a pointer to that in-core inode or
 * NULL if there are no more free in-core inodes. */
struct inode *incore_find_free(void)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        if (incore[i].ref_count == 0)
            return &incore[i];
    }
    return NULL;
}

/* This finds an in-core inode record in the incore array
 * by the inode number. It returns a pointer to that in-core
 * inode or NULL if it can't be found. */
struct inode *incore_find(unsigned int inode_num)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        if (incore[i].ref_count > 0 && incore[i].inode_num == inode_num)
            return &incore[i];
    }
    return NULL;
}

/* Sets the ref_count to all in-core inodes to 0. */
void incore_free_all(void)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        incore[i].ref_count = 0;
    }
}

/* This takes a pointer to an empty struct inode
 * that you're going to read the data into. The
 * inode_num is the number of the inode you wish to
 * read from disk. */
void read_inode(struct inode *in, int inode_num)
{
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    unsigned char block[BLOCK_SIZE];
    bread(block_num, block);

    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    in->size = read_u32(block + block_offset_bytes);
    in->owner_id = read_u16(block + block_offset_bytes + 4);
    in->permissions = read_u8(block + block_offset_bytes + 6);
    in->flags = read_u8(block + block_offset_bytes + 7);
    in->link_count = read_u8(block + block_offset_bytes + 8);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
        in->block_ptr[i] = read_u16(block + block_offset_bytes + 9 + i * 2);
    in->inode_num = inode_num;
}

/* This stores the inode data pointed to by in on
 * disk. The inode_num field in the struct holds
 * the number of the inode to be written. */
void write_inode(struct inode *in)
{
    int inode_num = in->inode_num;
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    unsigned char block[BLOCK_SIZE];
    bread(block_num, block);

    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    write_u32(block + block_offset_bytes, in->size);
    write_u16(block + block_offset_bytes + 4, in->owner_id);
    write_u8(block + block_offset_bytes + 6, in->permissions);
    write_u8(block + block_offset_bytes + 7, in->flags);
    write_u8(block + block_offset_bytes + 8, in->link_count);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
        write_u16(block + block_offset_bytes + 9 + i * 2, in->block_ptr[i]);
    bwrite(block_num, block);
}

/* Return a pointer to an in-core inode for the given
 * inode number, or NULL on failure. */
struct inode *iget(int inode_num)
{
    struct inode *in = incore_find(inode_num);
    if (in)
    {
        in->ref_count++;
        return in;
    }

    in = incore_find_free();
    if (in == NULL)
        return NULL;

    read_inode(in, in->inode_num);
    in->ref_count = 1;
    in->inode_num = inode_num;
    return in;
}

/* Decrement the reference count on the inode. If it
 * falls to 0, write the inode to disk. */
void iput(struct inode *in)
{   
    if (in->ref_count == 0)
        return;
    in->ref_count--;
    if (in->ref_count == 0)
        write_inode(in);
}

/* Debugging function */
void pinode(struct inode *in)
{
    printf("Inode Num: %d\n", in->inode_num);
    printf("  Size: %d\n", in->size);
    printf("  Owner: %d\n", in->owner_id);
    printf("  Perms: %d\n", in->permissions);
    printf("  Flags: %d\n", in->flags);
    printf("  Link: %d\n", in->link_count);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
        printf("  Blk Ptr %d: %d\n", i, in->block_ptr[i]);
}