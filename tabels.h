//
// Created by ניב on 05/04/2024.
//

#ifndef PROJECTC_TABELS_H
#define PROJECTC_TABELS_H
#include "globals.h"
#define TABLE_SIZE 64

typedef struct macro_table_content{
    char *macro_name;
    char *lines;
    struct macro_table_content *next;
}macro_table_content;
typedef struct{
    macro_table_content **entries;
}macro_table;
size_t hash(const char *key);
void free_macro_table(macro_table *hash);
void print_macro_table(const macro_table *table);
void delete_macro_content(macro_table *table, const char *macro_name);
char *get_lines(const macro_table *table, const char *macro_name);
void insert_macro_content(macro_table *table,const char *macro_name,const char *lines);
macro_table_content *create_macro_content(const char *macro_name,const char *lines);
macro_table *create_macro_table();
macro_table_content *get_macro_content(const macro_table *table, const char *macro_name);
typedef enum sym_types{
    entry_sym=0,
    data_sym,
    extern_sym,
    code_sym
}sym_types;
typedef struct sym_table_content{
    char *symbol;
    int symbol_address;
    sym_types sym_t;
    struct sym_table_content *next;
}sym_table_content;
typedef struct{
    sym_table_content **entries;
}sym_table;
sym_table *create_sym_table();
void destroy_symbol_table(sym_table *table);
void dump_symbol_table(const sym_table *table);
void delete_sym_content(sym_table *table,const char *sym_name);
sym_table_content *find_sym_content(const sym_table *table,const char *sym_name);
void insert_symbol_content(sym_table *table,const char *sym_name,int sym_add,sym_types sym_t);
sym_table_content *create_sym_content(const char *sym_name,int sym_add,sym_types sym_t);
#endif //PROJECTC_TABELS_H
