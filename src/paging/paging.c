#include "paging.h"
#include "memory/heap/kheap.h"

extern void paging_load_directory(uint32_t* pgd);

/*current page global directory mmu working on*/
static uint32_t* current_pgd = 0;

/*Initialize and allocate memory for pdg and page table(pt)*/
struct paging_4gb* create_paging_4gb(uint8_t flags)
{
    uint32_t* pgd = kzalloc(sizeof(uint32_t) * PAGING_TABLE_ENTRIES_PER_PGD); /*4bytes * 1024*/
    //starting address where physical memory starts, currently it is considered to be starting fron 0
    int offset = 0;
    //pde = page directory entry
    for(uint32_t pde = 0; pde < PAGING_TABLE_ENTRIES_PER_PGD; pde++){
        uint32_t* pt = kzalloc(sizeof(uint32_t) * PAGING_PAGES_PER_PAGE_TABLE);
        //pte = page table entry
        for(uint32_t pte = 0; pte < PAGING_PAGES_PER_PAGE_TABLE; pte++){
            //linear mapping of pte with physical memory address shifted by offset
            /*refer to page table entry structure for more info: https://wiki.osdev.org/Paging */
            pt[pte] = (offset + (pte * PAGING_PAGE_SIZE)) | flags ;
        }
        //increment to offset 1024(pte in one page table)*1024(one page_size)
        offset += (PAGING_PAGES_PER_PAGE_TABLE * PAGING_PAGE_SIZE);
        //each pde will points to pt entry created above
        /*refer to page directory entry structure for more info: https://wiki.osdev.org/Paging */
        pgd[pde] = (uint32_t)pt | flags | PAGING_IS_WRITEABLE;
    }

    //return the paging structure 
    struct paging_4gb* paging_struct = kzalloc(sizeof(struct paging_4gb));
    paging_struct->pgd = pgd;
    return paging_struct;
}
/*loads the pgd into cr3 register, mmu reference from this directory entry only*/
void paging_switch(uint32_t* pgd)
{
    paging_load_directory(pgd);
    current_pgd = pgd;
}

/*get the pgd from paging_4gb struct*/
uint32_t* paging_4gb_get_pgd(struct paging_4gb* paging_struct)
{
    return paging_struct->pgd;
}