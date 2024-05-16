#include <stdio.h>
#include "image.h"
#include "block.h"
#include "ctest.h"


#ifdef CTEST_ENABLE

void test_image(void) {
    CTEST_ASSERT(image_open("dummy.jpg", 1) == image_fd, "Testing non-existing file");
    image_close();
    CTEST_ASSERT(image_open("dummy.jpg", 1) == image_fd, "Testing existing file");
    image_close();
    remove("dummy.jpg");
}

int main(void) {
    CTEST_VERBOSE(1);

    test_image();

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