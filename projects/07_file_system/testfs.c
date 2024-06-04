#include <stdio.h>
#include "image.h"
#include "block.h"
#include "ctest.h"


#ifdef CTEST_ENABLE

void test_block() {
    // int block_num = 1;
    // unsigned char block[BLOCK_SIZE] = {0};
    // for (int i = 0; i < BLOCK_SIZE; i++)
    //     if (i % 3 == 0)
    //         block[i] = 1;

    // bwrite(block_num, &block);
    // unsigned char block2[BLOCK_SIZE];
    // bread(block_num, &block2);

    // CTEST_VERBOSE(0);
    // for (int i = 0; i < BLOCK_SIZE; i++)
    //     CTEST_ASSERT(block[i] == block2[i], "block: Testing read/write");
    // CTEST_VERBOSE(1);
}

void test_image(void) {
    CTEST_ASSERT(image_open("dummy.jpg", 1) == image_fd,
                 "image: Testing non-existing file");
    image_close();
    CTEST_ASSERT(image_open("dummy.jpg", 1) == image_fd,
                 "image: Testing existing file");
    image_close();
    remove("dummy.jpg");
}

void test_free(void) {

}

int main(void) {
    CTEST_VERBOSE(1);

    test_block();
    test_image();
    test_free();

    CTEST_RESULTS();

    CTEST_EXIT();
}

#else

int main(void) {
    int fd = image_open("dummy.jpg", 1);
    image_close();
    return 1;
}

#endif