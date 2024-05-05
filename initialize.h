//
// Created by ניב on 21/04/2024.
//

#ifndef COMPILERFINALPROJECT_INITIALIZE_H
#define COMPILERFINALPROJECT_INITIALIZE_H
#include "globals.h"
#include "utils.h"
#include "tabels.h"
#include "lexer.h"
void insert_string(char *str,data_word *data_img,long *dc);
void insert_data_number(int num,data_word *data_im,long *dc);
bool insert_opcode(line_description *line,struct lexer_tree *lex,machine_word *code_img,long *ic,sym_table *table);

#endif //COMPILERFINALPROJECT_INITIALIZE_H
