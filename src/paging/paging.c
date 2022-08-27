#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"

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

/*get virt addr pde and pte*/
/*
example: Vaddr: 0b0101000000000000 == 4214784 (dec) == 1029*4096
Need PDE = 1 and PTE = 9
formula:
pde = vaddr /(total_pte_in_one_pde*page_size) == (1029*4096)/(1024*4096) = 1
pte = (vaddr % (total_pte_in_one_pde*page_size) / page_size) == ((1029*4096) %(1024*4096))/4096 = 9
*/
static int paging_get_indexes(void* virtual_address, uint32_t* pde_index, uint32_t* pte_index)
{
    int res=0;

    if(!paging_is_aligned(virtual_address)){
        res = -EINVARG;
        goto out;
    }

    *pde_index = ((uint32_t)virtual_address / (PAGING_PAGES_PER_PAGE_TABLE * PAGING_PAGE_SIZE));
    *pte_index = ((uint32_t)virtual_address % (PAGING_PAGES_PER_PAGE_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);

out:
    return res;
}

/*set the virtual points to the physical addr provided as val*/
int paging_set(uint32_t* directory, void* virt, uint32_t val)
{
    uint32_t pde_index = 0;
    uint32_t pte_index = 0;

    int res = paging_get_indexes(virt,&pde_index,&pte_index);
    if(res < 0){
        return res;
    }

    uint32_t entry = directory[pde_index]; //pde
    uint32_t* table = (uint32_t*)(entry & 0xfffff000); //20bits as pde = pt 
    table[pte_index] = val;

    return 0;
}
/*check if addr given is page alinged*/
bool paging_is_aligned(void* addr)
{
    return ((uint32_t)addr % PAGING_PAGE_SIZE) == 0;
}