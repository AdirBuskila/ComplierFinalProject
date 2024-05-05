//
// Created by ניב on 08/04/2024.
//

#ifndef _ASM_FILES_H
#define _ASM_FILES_H
#include "globals.h"
#include "utils.h"
#include "tabels.h"
void write_obj_file(char *file_name,machine_word code_im[],long ic,data_word data_im[],long dc);
void write_entry_file(char *file_name,sym_table *table);
void write_extern_file(char *file_name,sym_table *table,machine_word code_img[],long ic);
#endif //_ASM_FILES_H
