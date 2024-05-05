//
// Created by ניב on 05/04/2024.
//

#ifndef COMPLIERFINALPROJECT_UTILS_H
#define COMPLIERFINALPROJECT_UTILS_H
#include <ctype.h>
#include "globals.h"
#include "tabels.h"
#define SKIP_WHITE_SPACE(string,index) \
        for(;string[(index)]&&(isspace(string[index]));(++(index)))\
        ;
char* str_allocate_cat(const char* str1, const char* str2);
bool is_array(char *str);
bool is_label(char *str);
bool is_number(const char *str);
void print_error_msg(line_description *line, const char *format, ...);
char **split_string(const char *str, const char *delim, int *count);
void free_split_string(char **splited_str,int count);
void update_symbol_table(sym_table *table,long ic);
void free_code_image(machine_word code_img[],long ic);
#endif //COMPLIERFINALPROJECT_UTILS_H
