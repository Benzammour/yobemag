#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "fixtures/cpu_mmu.h"

static uint8_t cpu_setup(const uint8_t opcode, const uint16_t address, const uint8_t lhs, const uint8_t rhs, uint8_t* reg) {
    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = lhs;
    *reg = rhs;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;
    
    return actual;
}

Test(sbc_a_n, sbc_a_a_no_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x9F;
    const uint16_t address = rand() % (0xFFFF);
    const uint8_t a = 128;
    const uint8_t expected = 0;
    set_flag(0, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, a, &cpu.AF.bytes.high);

    // Z should be set, as our result is 0
    cr_expect(eq(u8, get_flag_bit(Z_FLAG), 1));

    // N should be set
    cr_expect(eq(u8, get_flag_bit(N_FLAG), 1));

    // H should NOT be set because of no half-borrow
    cr_expect(zero(u8, get_flag_bit(H_FLAG)));

    // C should NOT be set because of no borrow
    cr_expect(zero(u8, get_flag_bit(C_FLAG)));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, (address + 1)));
}

Test(sbc_a_n, sbc_a_b_only_half_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x98;
    const uint8_t a = 0b0100111;
    const uint8_t b = 0b0000111;
    const uint8_t expected = a - b - 1;
    const uint16_t address = rand() % (0xFFFF);
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, b, &cpu.BC.bytes.high);

    // Z should not be set, as our result is not 0
    cr_expect(zero(u8, get_flag_bit(Z_FLAG)));

    // N should always be set
    cr_expect(eq(u8, get_flag_bit(N_FLAG), 1));

    // H should be set because borrow occured
    cr_expect(eq(u8, get_flag_bit(H_FLAG), 1));

    // C should not be set because of no borrow
    cr_expect(zero(u8, get_flag_bit(C_FLAG)));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_c_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x99;
    const uint8_t a = 0b0000111;
    const uint8_t c = 0b0010101;
    const uint8_t expected = a - c - 1;
    const uint16_t address = rand() % (0xFFFF);
    set_flag(1, C_FLAG);
    
    uint8_t actual = cpu_setup(opcode, address, a, c, &cpu.BC.bytes.low);

    // Z should not be set, as our result is not 0
    cr_expect(zero(u8, get_flag_bit(Z_FLAG)));

    // N should always be set
    cr_expect(eq(u8, get_flag_bit(N_FLAG), 1));

    // H should not be set because no HB occured
    cr_expect(zero(u8, get_flag_bit(H_FLAG)));

    // C should be set because of borrow
    cr_expect(eq(u8, get_flag_bit(C_FLAG), 1));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_d_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x9A;
    const uint8_t a = rand() % (0xFF);
    const uint8_t d = rand() % (0xFF);
    const uint8_t expected = a - d - 1;
    const uint16_t address = rand() % (0xFFFF);
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, d, &cpu.DE.bytes.high);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_e_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x9B;
    const uint8_t a = rand() % (0xFF);
    const uint8_t e = rand() % (0xFF);
    const uint8_t expected = a - e - 1;
    const uint16_t address = rand() % (0xFFFF);
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, e, &cpu.DE.bytes.low);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_h_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x9C;
    const uint8_t a = rand() % (0xFF);
    const uint8_t h = rand() % (0xFF);
    const uint8_t expected = a - h - 1;
    const uint16_t address = rand() % (0xFFFF);
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, h, &cpu.HL.bytes.high);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_l_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x9D;
    const uint8_t a = rand() % (0xFF);
    const uint8_t l = rand() % (0xFF);
    const uint8_t expected = a - l - 1;
    const uint16_t address = rand() % (0xFFFF);
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, l, &cpu.HL.bytes.low);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_hl_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x9E;
    const uint8_t a = 70;
    const uint8_t num = 1;
    const uint8_t expected = a - num - 1;
    set_flag(1, C_FLAG);

    const uint16_t address = rand() % (0xFFFF);
    const uint16_t word_address = (rand() + 0x8000) % (0x10000);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.HL.word = word_address;
    mmu_write_byte(address, opcode);
    mmu_write_byte(word_address, num);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_d8_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0xDE;
    const uint8_t a = 70;
    const uint8_t num = 1;
    const uint8_t expected = a - num - 1;
    set_flag(1, C_FLAG);

    const uint16_t address = rand() % (0xFFFF - 1);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    mmu_write_byte(address, opcode);
    mmu_write_byte(address+1, num);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 2));
}
