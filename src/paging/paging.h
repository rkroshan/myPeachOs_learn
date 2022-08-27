#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdbool.h>

/*
https://wiki.osdev.org/Paging
*/

//includes
#include <stdint.h>
#include <stddef.h>

//1:cache disabled 0: cache enabled
#define PAGING_CACHE_DISABLED   0b00010000
//1:write-through 0:write-back
#define PAGING_WRITE_THROUGH    0b00001000
//1:access_all_rings 0:only_ring_0
#define PAGING_ACCESS_ALL_RINGS 0b00000100
//1:page writeable and reable 0: read only
#define PAGING_IS_WRITEABLE      0b00000010
//1:page is present in memory 0: not present trigger page fault
#define PAGING_IS_PRESENT       0b00000001

//32 bit system follows 2 level page table hirerarchy
/*
PGD(Page Global Directory) -> 1024 PDE(Page Directory Entries)
PDE == PT(Page Table)
PT -> 1024 PTE(Page Table Entries)
PTE == Page Frame (actual physical memory)
*/
#define PAGING_TABLE_ENTRIES_PER_PGD    1024
#define PAGING_PAGES_PER_PAGE_TABLE     1024
#define PAGING_PAGE_SIZE                4096

//structure definitions
struct paging_4gb{
    uint32_t* pgd; /*points to page global directory(pgd)*/
};

//Functions prototype
/*Initialize and allocate memory for pdg and page table(pt)*/
struct paging_4gb* create_paging_4gb(uint8_t flags);
/*loads the pgd into cr3 register, mmu reference from this directory entry only*/
void paging_switch(uint32_t* pgd);
/*enable the paging*/
void enable_paging(void);

/*set the virtual points to the physical addr provided as val*/
int paging_set(uint32_t* directory, void* virt, uint32_t val);
/*check if addr given is page alinged*/
bool paging_is_aligned(void* addr);

/*get the pgd from paging_4gb struct*/
uint32_t* paging_4gb_get_pgd(struct paging_4gb* paging_struct);

#endif