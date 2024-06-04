#include <unistd.h>
#include "block.h"

/* Takes a pointer to a buffer to load with the block data.
 * It copies it out of the disk image into that buffer.
 * Returns a pointer to that same buffer. */
unsigned char *bread(int block_num, unsigned char *block) {
    read(block_num, block, BLOCK_SIZE);
    return block;
}

/* Takes a pointer to a buffer full of block data to write. */
void bwrite(int block_num, unsigned char *block) {
    write(block_num, block, BLOCK_SIZE);
}

/* Allocate a previously-free data block from the block map */
int alloc(void) {
    unsigned char block;
    bread(DATA_BLOCK, block);
    int free_bit = find_free(block);
    if (free_bit != -1) {
        set_free(block, free_bit, 1);
        bwrite(DATA_BLOCK, block);
    }
    return free_bit;
}