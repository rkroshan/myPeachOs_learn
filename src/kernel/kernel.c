#include "kernel.h"
#include "display/vga/vga.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "paging/paging.h"


extern void raise_int_zero(void);

/*kernel paging_4g struct*/
static struct paging_4gb* kernel_paging_struct = 0;

static void kernel_init(){
    /*initialize the terminal*/
    terminal_initialize();

    /*Initialize the heap*/
    kheap_init();

    /*initialize the interrupt descriptor table*/
    idt_init();

    /*setup paging*/
    kernel_paging_struct = create_paging_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_ALL_RINGS);
    //switch to kernel paging directory
    paging_switch(paging_4gb_get_pgd(kernel_paging_struct));
    /*example of paging working
    1. allocate memory from heap before enableing paging
    2. map this physical address to some virtual addre
    3. enable paging
    4. whatever we write referencing the new ptr, same will can be printable from old ptr
    */
    char* old_ptr = kzalloc(4096);

    paging_set(paging_4gb_get_pgd(kernel_paging_struct),(void*)0x1000, (uint32_t)old_ptr | PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_ALL_RINGS);
    //enable paging
    enable_paging();

    char* new_ptr = (char*)0x1000;
    new_ptr[0] = 'A';
    new_ptr[1] = 'B';
    terminal_print(new_ptr);

    terminal_print(old_ptr);

    /*enable interrupts*/
    enable_interrupts();
}

void kernel_main()
{
    kernel_init();
    terminal_print("This is roshanOs........\nInitializing Please wait\nchecking..\n");
    // raise_int_zero();
    // outb(0x60, 0xff); //checking if outb works writing 0xff to 0x60 port
    // terminal_print("Able to write after interrupt raise\n");
    void *ptr1, *ptr2, *ptr3;
    ptr1 = kmalloc(200);
    ptr2 = kmalloc(8196);
    kfree(ptr1);
    ptr3 = kmalloc(400);
    if(ptr1 || ptr2 || ptr3){
        
    }
}