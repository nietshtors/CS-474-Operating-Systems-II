#include <stdio.h>
#include <string.h>
#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"
#include "dir.h"
#include "ls.c"
#include "ctest.h"

#ifdef CTEST_ENABLE

void test_image(void)
{
    remove("dummy.txt");
    CTEST_ASSERT(image_open("dummy.txt", 1) == image_fd,
                 "image: Testing non-existing file");
    image_close();
    CTEST_ASSERT(image_open("dummy.txt", 1) == image_fd,
                 "image: Testing existing file");
    image_close();
    remove("dummy.txt");
}

void test_block()
{
    int block_num = INODE_MAP;
    unsigned char block[BLOCK_SIZE] = {0};
    for (int i = 0; i < BLOCK_SIZE; i++)
        if (i % 3 == 0)
            block[i] = 1;
    bwrite(block_num, block);
    unsigned char block2[BLOCK_SIZE];
    bread(block_num, block2);

    CTEST_ASSERT(memcmp(block, block2, BLOCK_SIZE) == 0, "block: Read matches write");
}

void test_set_free(void)
{
    unsigned char block[BLOCK_SIZE] = {0};
    set_free(block, 0, 1);
    CTEST_ASSERT(block[0] == 0x01, "free: Setting bit 0 to 1");
    set_free(block, 7, 1);
    CTEST_ASSERT(block[0] == 0x81, "free: Setting bit 7 to 1");
    set_free(block, 15, 1);
    CTEST_ASSERT(block[1] == 0x80, "free: Setting bit 15 to 1");
    set_free(block, 31, 1);
    CTEST_ASSERT(block[3] == 0x80, "free: Setting bit 31 to 1");
    set_free(block, 7, 0);
    CTEST_ASSERT(block[0] == 0x01, "free: Setting bit 7 to 0");
}

void test_find_free(void)
{
    unsigned char block[BLOCK_SIZE];
    int bit = 340;
    for (int i = 0; i < bit; i++)
        set_free(block, i, 1);
    set_free(block, bit, 0);
    CTEST_ASSERT(find_free(block) == bit, "free: Finds correct free bit");
    bit = 200;
    set_free(block, bit, 0);
    CTEST_ASSERT(find_free(block) == bit, "free: Finds correct free bit");
}

void test_inode(void)
{
    struct inode *in = ialloc();
    CTEST_ASSERT(in == iget(in->inode_num), "inode: iget finds correct inode");
    in->size = 3;
    write_inode(in);
    struct inode *in2 = ialloc();
    read_inode(in2, in->inode_num);
    CTEST_ASSERT(in->size == in2->size, "inode: Read/write works");
}

void test_dir(void)
{
    remove("fs.txt");
    image_open("fs.txt", 1);

    mkfs();
    struct directory *dir = directory_open(0);
    struct directory *dir2 = directory_open(0);
    CTEST_ASSERT(dir->inode->inode_num == dir2->inode->inode_num, "dir: directory_open opens to correct spot");
    struct directory_entry ent;
    directory_get(dir, &ent);
    CTEST_ASSERT(ent.inode_num == 0 && strcmp(ent.name, ".") == 0, "dir: mkfs test 1");
    directory_get(dir, &ent);
    CTEST_ASSERT(ent.inode_num == 0 && strcmp(ent.name, "..") == 0, "dir: mkfs test 2");

    image_close();
}

int main(void)
{
    CTEST_VERBOSE(1);

    test_image();

    remove("fs.txt");
    image_open("fs.txt", 1);

    test_block();
    test_set_free();
    test_find_free();
    test_inode();

    image_close();

    test_dir();

    CTEST_RESULTS();
    CTEST_EXIT();
}

#else

int main(void)
{
    image_open("fs.txt", 1);
    mkfs();
    ls();
    image_close();
    return 1;
}

#endif