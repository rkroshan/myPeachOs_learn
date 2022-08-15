#ifndef __IO_H__
#define __IO_H__

//Function Prototypes
/*Read a byte from port*/
unsigned char insb(unsigned short port);
/*Read a word from port*/
unsigned short insw(unsigned short port);

/*write a byte to port*/
void outb(unsigned short port, unsigned char val);
/*write a word to port*/
void outw(unsigned short port, unsigned short val);

#endif