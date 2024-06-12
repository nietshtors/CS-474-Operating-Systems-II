#include <unistd.h>
#include <fcntl.h>
#include "image.h"

int image_fd;

/* Opens the image file of the given name, creating it
 * if it doesn't exist, and truncating it to 0 size if
 * truncate is true */
int image_open(char *filename, int truncate)
{
    if (truncate)
    {
        image_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0600);
    }
    else
    {
        image_fd = open(filename, O_RDWR | O_CREAT, 0600);
    }
    return image_fd;
}

/* Closes the image file */
int image_close(void)
{
    return close(image_fd);
}