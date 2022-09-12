#ifndef __FILE_H__
#define __FILE_H__

//includes
#include "pparser.h"

typedef unsigned int FILE_SEEK_MODE;
enum
{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef unsigned int FILE_MODE;
enum
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

//Function Pointers
struct disk; 
/*abstract to create fs_open function in each file system*/
typedef void* (*FS_OPEN_FUNCTION)(struct disk* disk, struct path_part* path, FILE_MODE mode);
/*abstract to create fe_reslove function in each file system*/
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* disk);

///Structure definitions
struct filesystem
{
    FS_RESOLVE_FUNCTION resolve ;   //resolve disk function of fileystem
    FS_OPEN_FUNCTION open;          //open function of filesystem
    char name[20];                  //name of the filesystem
};

struct file_descriptor
{
    int index;                      // The descriptor index
    struct filesystem* filesystem;  // File system to which it belongs
    void* private;                  // Private data for internal file descriptor
    struct disk* disk;              // The disk that the file descriptor shoulde be used on
};

//Function Prototypes
/*initialize file system*/
void fs_init();
/*Interface function to open file with given mode*/
int fopen(const char* filename, const char* mode);
/*Fill insert filesystem structure*/
void fs_insert_filesystem(struct filesystem* filesystem);
/*resolve filesystem for given disk*/
struct filesystem* fs_resolve(struct disk* disk);

#endif