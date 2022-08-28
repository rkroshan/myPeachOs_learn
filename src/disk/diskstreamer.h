#ifndef __DISKSTREAMER_H__
#define __DISKSTREAMER_H__

//includes
#include "disk.h"

//structure definitions
struct diskstreamer{
    int pos; //position in disk
    struct disk* disk; //disk structure
};

//Function Prototypes
/*Initialize disk streamer struct*/
struct diskstreamer* new_diskstreamer(int disk_id);
/*change diskstreamer pos variable*/
int diskstreamer_seek(struct diskstreamer* stream, int pos);
/*read specified total_bytes in out buffer*/
int diskstreamer_read(struct diskstreamer* stream, void* out, int total_bytes);
/*clear the structure from memory*/
void diskstreamer_close(struct diskstreamer* stream);

#endif