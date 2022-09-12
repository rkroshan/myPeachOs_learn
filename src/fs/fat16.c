#include "fat16.h"
#include "utils/strings/strings.h"
#include "disk/disk.h"
#include "disk/diskstreamer.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"

//Macros
#define FAT16_SIGNATURE         0x29
#define FAT16_FAT_ENTRY_SIZE    0x02
#define FAT16_BAD_SECTOR        0xFF7
#define FAT16_UNUSED            0x00

typedef unsigned int FAT_ITEM_TYPE;
#define FAT_ITEM_TYPE_DIRECTORY 0
#define FAT_ITEM_TYPE_FILE 1

// Fat directory entry attributes bitmask
#define FAT_FILE_READ_ONLY 0x01
#define FAT_FILE_HIDDEN 0x02
#define FAT_FILE_SYSTEM 0x04
#define FAT_FILE_VOLUME_LABEL 0x08
#define FAT_FILE_SUBDIRECTORY 0x10
#define FAT_FILE_ARCHIVED 0x20
#define FAT_FILE_DEVICE 0x40
#define FAT_FILE_RESERVED 0x80

//struct depicting EBPB (Extended Bios Parameter Block)
struct fat_header_extended
{
    uint8_t drive_number;
    uint8_t win_nt_bit;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_id_string[11];
    uint8_t system_id_string[8];
} __attribute__((packed));

//struct depicting BPB (Bios Parameter Block)
struct fat_header
{
    uint8_t short_jmp_ins[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_copies;
    uint16_t root_dir_entries;
    uint16_t number_of_sectors;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_setors;
    uint32_t sectors_big;
} __attribute__((packed));

//struct depicting complete BPB
//union to make an exception if EBPB not exist
struct fat_h
{
    struct fat_header primary_header;
    union fat_h_e
    {
        struct fat_header_extended extended_header;
    } shared;
};

//struct depicting fat directory item
struct fat_directory_item
{
    uint8_t filename[8];
    uint8_t ext[3];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t creation_time_tenths_of_a_sec;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access;
    uint16_t high_16_bits_first_cluster;
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t low_16_bits_first_cluster;
    uint32_t filesize;
} __attribute__((packed));

//struct depicting fat directory
struct fat_directory
{
    struct fat_directory_item* item;
    int total;
    int sector_pos;
    int ending_sector_pos;
};

//struct depeciting fat item, union to make sure it can be directoru or a item
struct fat_item
{
    union 
    {
        struct fat_directory_item* item;
        struct fat_directory* directory;
    };

    FAT_ITEM_TYPE type;
};

//struct depicting fat item descriptor
struct fat_item_descriptor
{
    struct fat_item* item;
    uint32_t pos;
};


struct fat_private
{
    struct fat_h header;
    struct fat_directory root_directory;

    // Used to stream data clusters
    struct diskstreamer* cluster_read_stream;
    // Used to stream the file allocation table
    struct diskstreamer* fat_read_stream;
    // Used in situations where we stream the directory
    struct diskstreamer* directory_stream;
};

//declarations
/*create fs_open function in each file system*/
void* fat16_open(struct disk* disk, struct path_part* path, FILE_MODE mode);
/*create fe_reslove function in each file system*/
int fat16_resolve(struct disk* disk);

//global declarations
struct filesystem fat16_fs = 
{
    .resolve = fat16_resolve,
    .open = fat16_open
};

/*return fat16 filesystem struct*/
struct filesystem* fat16_init()
{
    strcpy(fat16_fs.name,"FAT16");
    return &fat16_fs;
}

//static function declarations
/*
returns absolute position for the given sector
*/
static inline int fat16_sector_to_absolute(struct disk* disk, int sector)
{
    return (sector * disk->sector_size);
}

/*
Initialize the diskstreamers in the fat_private structure
*/
static void fat16_init_private(struct disk* disk, struct fat_private* private)
{
    memset(private, 0, sizeof(struct fat_private));
    private->cluster_read_stream = new_diskstreamer(disk->id);
    private->fat_read_stream = new_diskstreamer(disk->id);
    private->directory_stream = new_diskstreamer(disk->id);
}

/*
returns total items in the current directory, whether directory or a file
*/
static int fat16_get_total_items_for_directory(struct disk* disk, uint32_t directory_start_sector)
{
    struct fat_directory_item item;
    struct fat_directory_item empty_item;
    memset(&empty_item, 0, sizeof(empty_item));
    //get fat_private data structure
    struct fat_private* fat_private = disk->fs_private;

    int res = 0;
    int i = 0; //tack total items in the current directory
    //starting seek position from root_dir_sector
    int directory_start_pos = directory_start_sector * disk->sector_size;
    //get the directory streamer
    struct diskstreamer* streamer = fat_private->directory_stream;
    //set the seek pos to directory_start_pos
    if(diskstreamer_seek(streamer, directory_start_pos) != STATUS_OK)
    {
        res = -EIO;
        goto out;
    }

    //Read each entry struct one by one
    while(1)
    {
        if (diskstreamer_read(streamer, &item, sizeof(item)) != STATUS_OK)
        {
            res = -EIO;
            goto out;
        }
        //if the filename is NULL, that means we are done
        if (item.filename[0] == 0x00)
        {
            break;
        }

        // If the item unused, (deletion cases within directory, that entry will be marked unused)
        if (item.filename[0] == 0xE5)
        {
            continue;
        }

        i++;
    }

    res = i;

out:
    return res;
}

/*
Initialize root directory variables in fat_private structure
*/
static int fat16_get_root_directory(struct disk* disk, struct fat_private* fat_private, struct fat_directory* directory)
{
    int res = 0;
    //get the primary_header
    struct fat_header* primary_header = &fat_private->header.primary_header;
    //get root_dir_sector_pos
    int root_dir_sector_pos = (primary_header->fat_copies * primary_header->sectors_per_fat) + primary_header->reserved_sectors;
    //get total_no of root_dir entries allowed
    int root_dir_entries = fat_private->header.primary_header.root_dir_entries;
    //get total size to store these root_dir entries
    int root_dir_size = (root_dir_entries * sizeof(struct fat_directory_item));
    //get total sectors to store this much root_dir_size
    int total_sectors = root_dir_size / disk->sector_size;
    if (root_dir_size % disk->sector_size)
    {
        total_sectors += 1; //to compensate round of errors
    }

    //get total items in root directory
    int total_items = fat16_get_total_items_for_directory(disk, root_dir_sector_pos);
    //get root dir struct which holds all the root dir entries
    struct fat_directory_item* dir = kzalloc(root_dir_size);
    if (!dir)
    {
        res = -ENOMEM;
        goto out;
    }
    //get diskstreamer to read the dir entries
    struct diskstreamer* streamer = fat_private->directory_stream;
    if (diskstreamer_seek(streamer, fat16_sector_to_absolute(disk, root_dir_sector_pos)) != STATUS_OK)
    {
        res = -EIO;
        goto out;
    }
    //fill the dir struct which will be equal to total_root dir entries * struct fat_directory_item
    if (diskstreamer_read(streamer, dir, root_dir_size) != STATUS_OK)
    {
        res = -EIO;
        goto out;
    }

    directory->item = dir;
    directory->total = total_items;
    directory->sector_pos = root_dir_sector_pos;
    directory->ending_sector_pos = root_dir_sector_pos + (root_dir_size / disk->sector_size);
out:
    return res;
}

/*create fs_open function in each file system*/
void* fat16_open(struct disk* disk, struct path_part* path, FILE_MODE mode)
{
    return 0;
}
/*create fe_reslove function in each file system*/
int fat16_resolve(struct disk* disk)
{
    int res = 0;
    //allocate memory for fat_private structure
    struct fat_private* fat_private = kzalloc(sizeof(struct fat_private));
    fat16_init_private(disk, fat_private);
    disk->fs_private = fat_private;
    disk->filesystem = &fat16_fs; 

    /*Now lets read the Boot Record Sector*/
    struct diskstreamer* streamer = new_diskstreamer(disk->id);
    if(!streamer){
        res = -ENOMEM;
        goto out;
    }

    //read upto EBPB aka size of fat_private->header
    if (diskstreamer_read(streamer, &fat_private->header, sizeof(fat_private->header)) != STATUS_OK)
    {
        res = -EIO;
        goto out;
    }

    //Verify if the Volume Boot Record is actually fat16 partition or not
    if(fat_private->header.shared.extended_header.signature != FAT16_SIGNATURE)
    {
        res = -EFSNOTFAT16;
        goto out;
    }

    //Initialize root directory variables in fat_private structure
    if(fat16_get_root_directory(disk, fat_private, &fat_private->root_directory) != STATUS_OK)
    {
        res = -EIO;
        goto out;
    }

out:
    if(streamer)
    {
        diskstreamer_close(streamer);
    }

    if(res < 0)
    {
        kfree(fat_private);
        disk->fs_private = NULL;
    }
    return res;
}