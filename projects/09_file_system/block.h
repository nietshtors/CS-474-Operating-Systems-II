#ifndef BLOCK_H
#define BLOCK_H

#define BLOCK_SIZE 4096
#define SUPERBLOCK 0
#define INODE_MAP 1
#define DATA_MAP 2
#define INODE_DATA_0 3
#define INODE_DATA_1 4
#define INODE_DATA_2 5
#define INODE_DATA_3 6
#define FILE_DATA_START 7

unsigned char *bread(int block_num, unsigned char *block);
void bwrite(int block_num, unsigned char *block);
int alloc(void);

#endif