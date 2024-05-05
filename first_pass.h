//
// Created by ניב on 15/04/2024.
//

#ifndef COMPILERFINALPROJECT_FIRST_PASS_H
#define COMPILERFINALPROJECT_FIRST_PASS_H
#include "globals.h"
#include "tabels.h"
#include "lexer.h"
#include "initialize.h"
#include "utils.h"
bool first_pass(line_description line,sym_table *table,long *ic,long *dc, data_word *data_img,machine_word *code_img,struct lexer_tree *lex);
#endif //COMPILERFINALPROJECT_FIRST_PASS_H
