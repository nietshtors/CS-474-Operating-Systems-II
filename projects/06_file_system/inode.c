#include "inode.h"
#include "block.h"
#include "free.h"

/* Allocate a previously-free inode in the inode map */
int ialloc(void) {
    unsigned char block[BLOCK_SIZE];
    bread(INODE_BLOCK, block);
    int free_bit = find_free(block);
    if (free_bit != -1) {
        set_free(block, free_bit, 1);
        bwrite(INODE_BLOCK, block);
    }
    return free_bit;
}
