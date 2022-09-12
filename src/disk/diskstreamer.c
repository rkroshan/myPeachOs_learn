#include "diskstreamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

/*Initialize disk streamer struct*/
struct diskstreamer* new_diskstreamer(int disk_id)
{
    struct disk* disk = disk_get(disk_id);
    if(!disk) {return 0;}

    struct diskstreamer* streamer = kzalloc(sizeof(struct diskstreamer));
    streamer->disk = disk;
    streamer->pos = 0;
    return streamer;
}

/*change diskstreamer pos variable*/
int diskstreamer_seek(struct diskstreamer* stream, int pos)
{
    stream->pos = pos;
    return 0;
}

/*read specified total_bytes in out buffer*/
int diskstreamer_read(struct diskstreamer* stream, void* out, int total_bytes)
{
    /*
    example current pos: 42, total_bytes= 2006
    current_sector = 42/512 = 0 == lba
    final offset = 42+2006 = 2048 
    sector_upto_read = (final_offset - 1)/ 512 == 3
    total_sectors_to_read = sectors_to_read - current_sector +1 = 3-0+1 = 4
    */
    int current_sector = stream->pos / DISK_SECTOR_SIZE ;   // this will give current sector number (using as lba)
    int current_pos = stream->pos;
    int final_pos = stream->pos + total_bytes;              // final diskstream pos after read, so we have to read upto finaa_pos -1
    int sector_upto_read = (final_pos - 1)/ DISK_SECTOR_SIZE ;// number of sectors to read to reach to final position
    int total_sectors_to_read = sector_upto_read - current_sector + 1;
    //call disk_read_block to read the number of sectors required
    char* buf = kmalloc(DISK_SECTOR_SIZE*(total_sectors_to_read));
    int res = disk_read_block(stream->disk,current_sector,total_sectors_to_read,buf);
    if(res < 0){
        goto out;
    }

    //fill the out buffer to the required bytes
    int pos_offset_in_sector = current_pos % DISK_SECTOR_SIZE;
    for(int i=0 ;i<total_bytes; i++){
        *(char*)out++ = buf[pos_offset_in_sector + i];
    }

    //change the streamer seek position
    diskstreamer_seek(stream, final_pos);

out:
    //free the buffer memory from heap
    kfree(buf);
    //return the result
    return res;

}

/*clear the structure from memory*/
void diskstreamer_close(struct diskstreamer* stream)
{
    kfree(stream);
}
