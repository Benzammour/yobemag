#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"

typedef struct LD_8bit_TestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    uint8_t l_dword_reg;
    uint8_t l_word_offset;
    uint8_t r_dword_reg;
    uint8_t r_word_offset;
    uint8_t expected;
} LD_8bit_TestParams;

// LD B, n
ParameterizedTestParameters(ld_b_n, ld_b_n) {
    static LD_8bit_TestParams params[] = {
        { 0x40, 128, 128, offsetof(CPU, BC), offsetof(Register, bytes.high), offsetof(CPU, BC), offsetof(Register, bytes.high), 128 },
        { 0x41, 128, 1, offsetof(CPU, BC), offsetof(Register, bytes.high), offsetof(CPU, BC), offsetof(Register, bytes.low), 1 },
        { 0x42, 128, 2, offsetof(CPU, BC), offsetof(Register, bytes.high), offsetof(CPU, DE), offsetof(Register, bytes.high), 2 },
        { 0x43, 128, 3, offsetof(CPU, BC), offsetof(Register, bytes.high), offsetof(CPU, DE), offsetof(Register, bytes.low), 3 },
        { 0x44, 128, 4, offsetof(CPU, BC), offsetof(Register, bytes.high), offsetof(CPU, HL), offsetof(Register, bytes.high), 4 },
        { 0x45, 128, 5, offsetof(CPU, BC), offsetof(Register, bytes.high), offsetof(CPU, HL), offsetof(Register, bytes.low), 5 },
        { 0x47, 128, 6, offsetof(CPU, BC), offsetof(Register, bytes.high), offsetof(CPU, AF), offsetof(Register, bytes.high), 6 },
    };
    
    // generate parameter set
    return cr_make_param_array(LD_8bit_TestParams, params, sizeof(params) / sizeof(LD_8bit_TestParams));
}

ParameterizedTest(LD_8bit_TestParams *params, ld_b_n, ld_b_n, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    // setup cpu
    uint16_t address = rand() % (0xFFFF);
    cpu.PC = address;
    
    // setup cpu
    uint8_t* goal_reg = get_cpu_regs(params->l_dword_reg, params->l_word_offset);
    uint8_t* source_reg = get_cpu_regs(params->r_dword_reg, params->r_word_offset);
    *goal_reg = params->lhs;
    *source_reg = params->rhs;

    mmu_write_byte(address, params->opcode);

    // do the actual emulation
    cpu_step();

    // check if value is correct
    uint8_t actual = *goal_reg;
    cr_expect(eq(u8, actual, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(ld_b_n, ld_b_hl_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x46;
    const uint8_t b = 70;
    const uint8_t num = 7;
    const uint8_t expected = num;
    const uint16_t address = rand() % (0xFFFF);
    const uint16_t word_address = (rand() + 0x8000) % (0x10000);

    cpu.PC = address;

    // setup cpu
    cpu.PC = address;
    cpu.BC.bytes.high = b;
    cpu.HL.word = word_address;
    mmu_write_byte(address, opcode);
    mmu_write_byte(word_address, num);

    // do the actual emulation
    cpu_step();

    // check if value is correct
    const uint8_t actual = cpu.BC.bytes.high;
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}
