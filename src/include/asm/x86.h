// Routines to call special x86 instructions from C code
#ifndef INCLUDE_ASM_X86_h_
#define INCLUDE_ASM_X86_h_

#include "types.h"

// Write a byte out to the specified port.
static inline void 
outb(ushort port, uchar value)
{
  __asm__ volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline uchar
inb(ushort port)
{
  uchar data;
   
  __asm__ volatile("inb %1, %0" : "=a" (data) : "dN" (port));
  return data;
}

static inline uchar
inw(ushort port)
{
  uchar data;

  __asm__ volatile ("inw %1, %0" : "=a" (data) : "dN" (port));
  return data;
}

#endif  //INCLUDE_ASM_X86_h_