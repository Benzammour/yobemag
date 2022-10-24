/** @file */

#ifndef YOBEMAG_MEM_H
#define YOBEMAG_MEM_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "attributes.h"

#define ROM_LIMIT     (0x8000)
#define BOOT_ROM_SIZE (256)
#define MEM_SIZE      (65536)

void mmu_init(void);
ATTR_PURE uint8_t mmu_get_byte(uint16_t addr);
void mmu_write_byte(uint16_t dest_addr, uint8_t value);
ATTR_PURE uint16_t mmu_get_two_bytes(uint16_t addr);
void mmu_write_two_bytes(uint16_t dest_addr, uint16_t value);
void mmu_destroy(void);

#endif // YOBEMAG_MEM_H
