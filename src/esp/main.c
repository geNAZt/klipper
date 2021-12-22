// Main starting point for ESP boards.
//
// Copyright (C) 2021,2022 Fabian Faßbender <fabian.fassbender42@googlemail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include <stdint.h>
#include "board/irq.h"

#define WSR(reg, newval)  asm volatile ("wsr %0, " #reg : : "r" (newval));
#define WITLB(at, as)     asm volatile ("witlb  %0, %1; \n isync \n " : : "r" (at), "r" (as))
#define WDTLB(at, as)     asm volatile ("wdtlb  %0, %1; \n dsync \n " : : "r" (at), "r" (as))

// Main entry point for esp code.
void __attribute__((noreturn)) init_cpu0(void)
{
   // Set memory control for xchal
   WSR(97, 0x00000000); 

   // Protect important memory locations
   const int illegal_regions[] = {0, 4, 5, 6, 7}; // 0x00000000, 0x80000000, 0xa0000000, 0xc0000000, 0xe0000000
   for (int i = 0; i < sizeof(illegal_regions) / sizeof(illegal_regions[0]); ++i) 
   {
      uint32_t addr = illegal_regions[i] * 0x20000000U;
      WITLB(0xF, addr);
      WDTLB(0xF, addr);
   }

   WDTLB(0x0, 0x20000000U);

   // Now we should be ready to go
   irq_enable();
}
