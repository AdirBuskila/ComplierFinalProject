//
// Created by ניב on 28/04/2024.
//

#ifndef COMPILERFINALPROJECT_SECOND_PASS_H
#define COMPILERFINALPROJECT_SECOND_PASS_H
#include "globals.h"
#include "lexer.h"
#include "tabels.h"
#include "utils.h"

bool second_pass_process(struct lexer_tree lex,sym_table *table,line_description line);
void init_second_pass(sym_table *table,machine_word *code_img,long ic);

#endif //COMPILERFINALPROJECT_SECOND_PASS_H
