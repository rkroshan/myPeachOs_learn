#ifndef __DISK_H__
#define __DISK_H__

//includes
#include <stdint.h>
#include <stddef.h>
#include "fs/file.h"

typedef unsigned int DISK_TYPE;


// Represents a real physical hard disk
#define DISK_TYPE_REAL 0

struct disk
{
    DISK_TYPE type;
    int sector_size;
    int id; // The id of the disk
    struct filesystem* filesystem;
    // The private data of our filesystem
    void* fs_private;
};

//Function Prototypes
/*read total_sectors count from given lba and place it in buf*/
int disk_read_sector(uint32_t lba, uint16_t total_sectors, void* buf);
/*search and initialize the disk*/
void disk_search_and_init();
/*return the disk structure*/
struct disk* disk_get(int index);
/*API to read from the disk*/
int disk_read_block(struct disk* idisk, unsigned int lba, int total_sectors, void* buf);

#endif