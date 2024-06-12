#ifndef DIR_H
#define DIR_H

#define FLAGS_UNKNOWN 0
#define FLAGS_FILE 1
#define FLAGS_DIRECTORY 2
#define DIRECTORY_SIZE 32

struct directory
{
    struct inode *inode;
    unsigned int offset;
};

struct directory_entry
{
    unsigned int inode_num;
    char name[16];
};

void mkfs(void);
struct directory *directory_open(int inode_num);
int directory_get(struct directory *dir, struct directory_entry *ent);
void directory_close(struct directory *d);

#endif