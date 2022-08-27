#ifndef __DISK_H__
#define __DISK_H__

//includes
#include <stdint.h>
#include <stddef.h>

//Function Prototypes
/*read total_sectors count from given lba and place it in buf*/
uint8_t disk_read_sector(uint32_t lba, uint16_t total_sectors, void* buf);

#endif