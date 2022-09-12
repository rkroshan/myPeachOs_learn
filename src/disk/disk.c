#include "disk.h"
#include "io/io.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"

/*actual disk currently accessing*/
struct disk disk;

/*read total_bytes count from given lba and place it in buf*/
int disk_read_sector(uint32_t lba, uint16_t total_sectors, void* buf)
{
    /*read disk configuration*/
    outb(0x1F6,(lba >> 24) | 0xE0 ) ;                    // Port to send drive and bit 24 - 27 of LBA and select master drive
    outb(0x1F2, total_sectors);                 // port to send no of sectors
    outb(0x1F3, (unsigned char)(lba & 0xff));   // Port to send bit 0 - 7 of LBA
    outb(0x1F4, (unsigned char)(lba >> 8));     // Port to send bit 8 - 15 of LBA
    outb(0x1F5, (unsigned char)(lba >> 16));    // Port to send bit 16 - 23 of LBA
    outb(0x1F7, 0x20);                          // Command port , 0x20 : read wih retry
   
   //since we are going to write 2 bytes ata time
   unsigned short* ptr = (unsigned short*) buf;

   while(total_sectors--){
    //wait for buffer to be ready
    char c = insb(0x1F7);                        // get the data from 0x1f7 port
    //check if it is equal to 8, if equals then data is not ready to read, try again
    while(!(c & 0x08)) {
        c = insb(0x1F7);
    }

    //copy data from hard disk to buf 2 bytes at a time
    for(uint16_t i=0;i<256;i++){
        *ptr = insw(0x1F0);                     // data port
        ptr++;
    }
   }

    return 0;
}

/*search and initialize the disk*/
void disk_search_and_init()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = DISK_TYPE_REAL;
    disk.sector_size = DISK_SECTOR_SIZE;
    disk.id = 0;
    disk.filesystem = fs_resolve(&disk);
}

/*return the disk structure*/
struct disk* disk_get(int index)
{
    if (index != 0)
        return 0;

    return &disk;
}

/*API to read from the disk*/
int disk_read_block(struct disk* idisk, unsigned int lba, int total_sectors, void* buf)
{
    if (idisk != &disk)
    {
        return -EIO;
    }

    return disk_read_sector(lba, total_sectors, buf);
}