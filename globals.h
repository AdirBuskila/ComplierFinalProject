#ifndef _GLOBALS_H
#define _GLOBALS_H
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef enum boolean{
    FALSE=0,
    TRUE=1
}bool;
/* Register and Opcode specifications */
#define REG_COUNT 8 /* Number of registers in the CPU */
#define OPCODE_COUNT 16 /* Total count of different opcodes */

/* Assembly file formatting constraints */
#define MAX_LINE_LENGTH 81 /* Maximum length of a line in the assembly file, including the newline character */
#define MAX_LABEL_LENGTH 31 /* Maximum length of a label */
#define MAX_MACRO_NAME_LENGTH 31
#define INSTRUCTIONS_COUNT 4 /* Total number of instruction types (might need adjustment) */
#define MAX_INT_VALUE 8191
#define MIN_INT_VALUE -8192

/* Memory layout */
#define DATA_SEGMENT_START_ADDRESS 100 /* Start address for the data segment in memory */
#define MAX_SYMBOL_TABLE_SIZE 4096 /* Maximum number of entries in the symbol table */
#define MAX_MACRO_NAME_LENGTH 31 /* Maximum length for macro names */
#define MAX_DATA_NUMBER 2048 /* Assumed maximum number of data entries (may need adjustment based on specifications) */
#define MEMORY_SIZE 4096 /* Total size of the memory in words */

/* File extensions for assembler output */
#define AS_FILE_EXTENSION ".as" /* Source assembly file extension */
#define AM_FILE_EXTENSION ".am" /* Preprocessed source file extension */
#define OB_FILE_EXTENSION ".ob" /* Object file extension containing the machine code */
#define ENT_FILE_EXTENSION ".ent" /* Entry symbols file extension */
#define EXT_FILE_EXTENSION ".ext" /* External symbols file extension */

/* Encoding of ERA (Extern/Relocatable/Absolute) in machine code words */
#define ERA_ABSOLUTE 0 /* Indicates an absolute address (no relocation needed) */
#define ERA_RELOCATABLE 2 /* Indicates a relocatable address */
#define ERA_EXTERNAL 1 /* Indicates an external symbol reference */

/* Error codes for assembly process */
#define ERROR_NONE 0 /* No error */
#define ERROR_SYNTAX 1 /* Syntax error detected */
#define ERROR_UNDEFINED_SYMBOL 2 /* Referenced symbol is undefined */
#define ERROR_DUPLICATE_SYMBOL 3 /* Symbol is defined more than once */

/* Specifications for output file formats */
#define OBJECT_FILE_HEADER_FORMAT "%d %d\n" /* Format string for the header of the object file (code size, data size) */
#define OBJECT_FILE_LINE_FORMAT "%04d %07d\n" /* Format string for each line of the object file (address, machine code) */

/* Limitations for command line interface */
#define MAX_INPUT_FILES 10 /* Maximum number of input files that can be processed in a single run */
typedef enum registers{
   r0=0,
   r1,
    r2,
    r3,
    r4,
   r5,
    r6,
    r7
}reg;

typedef enum op_code{
    cmd_mov=0,
    cmd_cmp,
    cmd_add,
    cmd_sub,
    cmd_not,
    cmd_clr,
    cmd_lea,
    cmd_dec,
    cmd_jmp,
    cmd_bne,
    cmd_red,
    cmd_prn,
    cmd_jsr,
    cmd_rts,
    cmd_hlt
}op_code;
enum operand_addr_type{
    add_immed=0,
    add_label,
    add_label_index,
    add_register
};
typedef struct first_word{
    unsigned int are_bits: 2;
    unsigned int dest_opcode_add_bits: 2;
    unsigned int src_opcode_add_bits: 2;
    unsigned int opcode_bits: 4;
    unsigned int not_used_bits: 4;
}first_word;
typedef struct immd_or_direct_word{
    unsigned int are_bits: 2;
    unsigned int opreand_bits: 12;
}immd_or_direct_word;
typedef struct reg_word{
    unsigned int are_bits: 2;
    unsigned int reg_dest_bits: 3;
    unsigned int regs_src_bits: 3;
    unsigned int not_used_bits: 6;
}reg_word;
typedef struct machine_word{
    char *label_name;
    union type_word{
        first_word *first_word;
        immd_or_direct_word *imm_or_dir_word;
        reg_word *reg_word;
    }type_word;
}machine_word;
typedef struct data_word{
    unsigned int data: 14;
}data_word;
typedef struct line_description{
    char *file_name;
    int  line_number;
    char *content;
}line_description;
#endif
