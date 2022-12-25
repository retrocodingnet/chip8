#include "chip8.h"
#include "chip8_errors.h"
#include "chip8_internal.h"

#include <memory.h>
#include <string.h>

static u8 fonts[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

int init_machine(struct machine_t *m) {
  memset(m, 0, sizeof(struct machine_t));
  
  m->cpu.PC = 0x200;
  memcpy(m->memory, fonts, sizeof(fonts));
  return chip8_success;
}

int load_machine(struct machine_t *m, void *program, size_t program_length) {
  if (program_length == 0) {
    return chip8_err_invalid_program;
  }

  init_machine(m);
  memcpy(m->memory, program, program_length);

  return chip8_success;
}

static int decode(inst_t instruction, struct inst_field_t *field) {
  memcpy(field, &instruction, sizeof(inst_t));

  return chip8_success;
}

static int execute_field(struct machine_t *m, struct inst_field_t field, chip8_callback_t cb) {
  int ret = cb(m, field);
  m->cpu.PC += 2;

  return ret;
}

