#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"

typedef struct TestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    size_t rhs_dreg_offset;
    size_t rhs_reg_offset;
    uint8_t expected;
    int address_increment;
    uint8_t F;
} TestParams;

typedef struct SpecialTestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    uint8_t expected;
    uint8_t F;
    bool is_HL;
} SpecialTestParams;

static void run_test(TestParams const *const params) {
    // setup cpu
    uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    cpu.PC           = address;
    CPU_REG_A        = params->lhs;

    // write rhs to desired register
    uint8_t *target_reg = get_cpu_reg(params->rhs_dreg_offset, params->rhs_reg_offset);
    *target_reg         = params->rhs;

    mmu_write_byte(address, params->opcode);

    // do the actual emulation
    cpu_step();

    // check flag register
    cr_expect(eq(u8, CPU_REG_F, params->F), "l: %d, r: %d, op: 0x%x", params->lhs, params->rhs,
              params->opcode);

    // check if value is correct
    uint8_t actual = CPU_REG_A;
    cr_expect(eq(u8, actual, params->expected), "l: %d, r: %d, op: 0x%x", params->lhs, params->rhs,
              params->opcode);

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, (address + params->address_increment)));
}

ParameterizedTestParameters(AND_A_n, AND_A_n_msb) {
    static TestParams params[] = {
        {0xA0, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
        {0xA1, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00100000},
        {0xA2, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
        {0xA3, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00100000},
        {0xA4, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
        {0xA5, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00100000},
        {0xA7, 128, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, AND_A_n, AND_A_n_msb, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    run_test(params);
}

ParameterizedTestParameters(AND_A_n, AND_A_n_alternating) {
    static TestParams params[] = {
        {0xA0, 0b10101010, 0b01010101, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 0,          1, 0b10100000},
        {0xA1, 0b10101010, 0b01010101, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 0,          1, 0b10100000},
        {0xA2, 0b10101010, 0b01010101, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 0,          1, 0b10100000},
        {0xA3, 0b10101010, 0b01010101, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 0,          1, 0b10100000},
        {0xA4, 0b10101010, 0b01010101, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 0,          1, 0b10100000},
        {0xA5, 0b10101010, 0b01010101, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 0,          1, 0b10100000},
        {0xA7, 0b10101010, 0b01010101, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 0b01010101, 1,
         0b00100000                                                                                                   },
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, AND_A_n, AND_A_n_alternating, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    run_test(params);
}

ParameterizedTestParameters(AND_A_n, AND_A_n_full) {
    static TestParams params[] = {
        {0xA0, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
        {0xA1, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00100000},
        {0xA2, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
        {0xA3, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00100000},
        {0xA4, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
        {0xA5, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00100000},
        {0xA7, 255, 255, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, AND_A_n, AND_A_n_full, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    run_test(params);
}

ParameterizedTestParameters(AND_A_n, AND_A_HL_and_d8) {
    static SpecialTestParams params[] = {
        {0xA6, 255, 255, 255, 0b00100000, true },
        {0xA6, 8,   8,   8,   0b00100000, true },
        {0xA6, 128, 128, 128, 0b00100000, true },
        {0xA6, 254, 1,   0,   0b10100000, true },
        {0xA6, 0,   0,   0,   0b10100000, true },
        {0xA6, 128, 129, 128, 0b00100000, true },

        {0xE6, 255, 255, 255, 0b00100000, false},
        {0xE6, 8,   8,   8,   0b00100000, false},
        {0xE6, 128, 128, 128, 0b00100000, false},
        {0xE6, 254, 1,   0,   0b10100000, false},
        {0xE6, 0,   0,   0,   0b10100000, false},
        {0xE6, 128, 129, 128, 0b00100000, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, AND_A_n, AND_A_HL_and_d8, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    uint8_t opcode             = params->opcode;
    uint8_t A                  = params->lhs;
    uint8_t value              = params->rhs;
    uint16_t address           = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t address_increment = 1;

    // setup cpu
    cpu.PC    = address;
    CPU_REG_A = A;
    mmu_write_byte(address, opcode);
    if (params->is_HL) {
        uint16_t indirection_address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
        CPU_DREG_HL                  = indirection_address;
        mmu_write_byte(indirection_address, value);
    } else {
        mmu_write_byte(address + 1, value);
        ++address_increment;
    }

    // do the actual emulation
    cpu_step();

    // check flag register
    cr_expect(eq(u8, CPU_REG_F, params->F));

    // check if value is correct
    cr_expect(eq(u8, CPU_REG_A, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + address_increment));
}
