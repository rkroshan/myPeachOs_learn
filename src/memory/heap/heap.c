#include "heap.h"
#include "status.h"
#include "memory/memory.h"


//static function
/*checks if ptr is aligned to HEAP_BLOCK_SIZE*/
static int heap_validate_alignment(void* ptr){
    return (((unsigned int)ptr % HEAP_BLOCK_SIZE) == 0);
}
/*validate the table size whether equals to heap size provided*/
static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    //heap size
    size_t heap_size = (size_t)(end - ptr);
    size_t total_heap_blocks = heap_size / HEAP_BLOCK_SIZE ; 

    return ((table->total_entries == total_heap_blocks)? STATUS_OK : -EINVARG);
}

/*calculate total number pf heap blocks required*/
static size_t heap_get_total_blocks(size_t size)
{
    return ((size_t)(((size-1)/HEAP_BLOCK_SIZE) + 1));
}
/*return status whether block is free or taken*/
static HEAP_BLOCK_TABLE_ENTRY heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry){
    return (HEAP_BLOCK_TABLE_ENTRY)(entry & 0x0F);
}

/*get the starting block index to fulfil given block_size requirement*/
static size_t heap_get_start_block(struct heap* heap, size_t total_blocks){
    struct heap_table* table = heap->table;
    int block_count=0;
    int start_block=-1;

    for(size_t i=0; i< table->total_entries; i++){
        //if we got total blocks that we need
        if((block_count == total_blocks)&&(start_block != -1)) return start_block;

        //If block is not free then reset the tracking variables
        if(heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE){
            start_block = -1;
            block_count = 0;
            continue;
        }
        else{
            //it means block is free 
            //check if start block index is not set
            if(start_block == -1) start_block = i;
            //increment the block count
            block_count++;
        }
    }

    //if start_block is not set to proper index return no memory available
    if(start_block == -1) return -ENOMEM;
    //else retuen start_block index
    return start_block;
}

/*convert block index to heap address*/
static inline void* heap_block_to_address(struct heap* heap,size_t start_block){
    return (void*)(heap->saddr + (start_block*HEAP_BLOCK_SIZE));
}

/*convert heap address to block index*/
static inline size_t heap_address_to_block(struct heap* heap,void* address)
{
    return ((size_t)(address - heap->saddr)/HEAP_BLOCK_SIZE);
}

/*Mark block entries in heap table as free*/
static void heap_mark_blocks_free(struct heap* heap, size_t starting_block)
{
    struct heap_table* table = heap->table;
    size_t i = starting_block;

    while(i < table->total_entries){
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if(!(entry & HEAP_BLOCK_HAS_NEXT)){
            //if the entry didn't had has_next mark that means it is the last block, so return in the mem allocation
            return;
        }
        i++;
    }
}

/*Mark block entries in heap_table as taken*/
static void heap_mark_blocks_taken(struct heap* heap,size_t start_block,size_t total_blocks)
{
    struct heap_table* table = heap->table;
    size_t end_block = start_block + total_blocks - 1;
    for(size_t i=start_block; i<= end_block;i++){
        //if it is the first block then mark it in the entries as first block and taken
        if(i == start_block){
            //check if there are more than one blocks then mark entry as has next
            if(total_blocks > 1){
                table->entries[i] = HEAP_BLOCK_HAS_NEXT | HEAP_BLOCK_IS_FIRST | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
            }else{
                table->entries[i] = HEAP_BLOCK_IS_FIRST | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
            }

        }else if( i <= (end_block - 1)){
            //if it not first block and neither last block then mark as has_next and taken
            table->entries[i] = HEAP_BLOCK_HAS_NEXT | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        }else{
            //it is the end block then mark as taken
            table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        }
        
    }
}

/*
heap_create : will create heap structure filled with heap entries as FREE blocks and setup the starting addres and size of heap
*/
int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{
    int result = 0;
    /*validate whether ptr and end have valid arguments i.e. it is multiple of HEAP_BLOCK_SIZE or not*/
    if(!heap_validate_alignment(ptr) || !(heap_validate_alignment(end)))
    {
        result = -EINVARG; //invalind argument
        goto out;
    }

    /*memset the heap struct*/
    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    /*validate the table size whether equals to heap size provided*/
    result = heap_validate_table(ptr,end,table);
    if(result < 0) goto out; //Invalid Argument

    /*clear the table entries data*/
    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY)*(table->total_entries);
    memset(table->entries,0,table_size);

out:
    return result;

}
/*
heap_ malloc : will allocate memory of size nearest to HEAP_BLOCK_SIZE multiple
*/
void* heap_malloc(struct heap* heap, size_t size)
{
    void* address = NULL;
    if(size <=0) return NULL; //Invalid Argument

    //get the nearest ceil total number of HEAP_BLOCKS required for fulfilling this allocation
    size_t total_blocks = heap_get_total_blocks(size);

    //Find starting block address in heap if possible to allocate total_blocks
    int start_block = heap_get_start_block(heap, total_blocks);
    if(start_block < 0){
        return NULL ; //unable to find way to allocate memory using this algorithm for given total blocks as of now
    }

    //Convert block index to address
    address = heap_block_to_address(heap,start_block);

    //Mark to provided blocks as taken
    heap_mark_blocks_taken(heap,start_block,total_blocks);

    //return the address
    return address;
}
/*
heap_free : will free the allocated memory
*/
void heap_free(struct heap* heap, void* ptr)
{
    //check if ptr is actually a valid block address
    if(!heap_validate_alignment(ptr)) return; //Invalid Argument, we can do assertion also to prevent mem leaks
    //mark the blocks as free
    heap_mark_blocks_free(heap,heap_address_to_block(heap,ptr));
}