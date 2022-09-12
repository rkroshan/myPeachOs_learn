#include "file.h"
#include "config.h"
#include "status.h"
#include "utils/strings/strings.h"
#include "display/vga/vga.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "fat16.h"

//global declaration
struct filesystem* filesystems[PEACHOS_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

//static functions
/*load the fat16 file system*/
static void fs_fat16_load()
{
    fs_insert_filesystem(fat16_init());
}

/*load the file systems*/
static void fs_load()
{
    //memset the file systems structs
    memset(filesystems, 0, sizeof(filesystems));
    /*load the Fat16 file system of now*/
    fs_fat16_load();
}

/*set free file_descriptor struct*/
static int file_new_descriptor(struct file_descriptor** desc_out)
{
    int res = -ENOMEM;
    for (int i = 0; i < PEACHOS_MAX_FILE_DESCRIPTORS; i++)
    {
        if( file_descriptors[i] == 0)
        {
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            desc->index = i+1; //Descriptor starts at 1
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = 0;
            break;
        }
    }

    return res;
}

/*get the free filesystem struct*/
static struct filesystem** fs_get_free_filesystem()
{
    int i=0;
    for(i =0; i < PEACHOS_MAX_FILESYSTEMS; i++)
    {
        if(filesystems[i] == 0){
            return &filesystems[i];
        }
    }
    return 0;
}

/*initialize file system*/
void fs_init()
{
    //memset the file descriptors
    memset(file_descriptors, 0, sizeof(file_descriptors));
    /*load the file descriptors*/
    fs_load();

}

/*Interface function to open file with given mode*/
int fopen(const char* filename, const char* mode)
{
    return -ENIY; //not implemented yet
}

/*Fill insert filesystem structure*/
void fs_insert_filesystem(struct filesystem* filesystem)
{
    struct filesystem** fs;
    fs = fs_get_free_filesystem();
    if(!fs){
        terminal_print("Problem in inserting filesystem");
        while(1){}
    }

    *fs = filesystem;
}

/*resolve filesystem for given disk*/
struct filesystem* fs_resolve(struct disk* disk)
{
    struct filesystem* fs = 0;
    for (int i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++)
    {
        if(filesystems[i] != 0 && (filesystems[i]->resolve(disk) == 0))
        {
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}