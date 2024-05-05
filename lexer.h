//
// Created by ניב on 26/04/2024.
//

#ifndef COMPILERFINALPROJECT_LEXER_H
#define COMPILERFINALPROJECT_LEXER_H
#include "globals.h"
#include "utils.h"

enum inst_type {
    /* Group a- mov,cmp,lea ,sub ,add */
    lex_inst_mov,
    lex_inst_cmp,
    lex_inst_add,
    lex_inst_sub,
    lex_inst_not,
    lex_inst_clr,
    lex_inst_lea,
    /*Group b - not, clr, inc, dec, jmp, bne, red, prn, jsr */
    lex_inst_inc,
    lex_inst_dec,
    lex_inst_jmp,
    lex_inst_bne,
    lex_inst_red,
    lex_inst_prn,
    lex_inst_jsr,
    /*group c- rts,hlt */
    lex_inst_rts,
    lex_inst_hlt
};
//struct cmd{
//    char *inst_name;
//    enum inst_type opdcode;
//};
// struct cmd list_cmd[] = {
//         {"mov", lex_inst_mov},
//         {"cmp", lex_inst_cmp},
//         {"add", lex_inst_add},
//         {"sub", lex_inst_sub},
//         {"not", lex_inst_not},
//         {"clr", lex_inst_clr},
//         {"lea", lex_inst_lea},
//         {"inc", lex_inst_inc},
//         {"dec", lex_inst_dec},
//         {"jmp", lex_inst_jmp},
//         {"bne", lex_inst_bne},
//         {"red", lex_inst_red},
//         {"prn", lex_inst_prn},
//         {"jsr", lex_inst_jsr},
//         {"rts", lex_inst_rts},
//         {"hlt", lex_inst_hlt}
//};
enum operand_address_type{
    lex_op_type_error=-1,
    lex_op_type_immed,
    lex_op_type_label,
    lex_op_type_index_const,
    lex_op_type_register
};
struct lexer_tree{
    char error[200];
    char label[MAX_LABEL_LENGTH];
    enum{
        lex_error=-1,
        lex_inst,
        lex_direct,
        lex_define,
        lex_empty
    }lex_tree_type;
    union {
        struct{
            char label[MAX_LABEL_LENGTH];
            int num;
        }define;
        struct{
            enum{
                lex_tree_extern,
                lex_tree_entry,
                lex_tree_string,
                lex_tree_data
            }direct_type;
            union {
                char label[MAX_LABEL_LENGTH];
                char string[MAX_LINE_LENGTH];
                struct{
                    enum{
                        data_label,
                        data_num
                    }data_type;
                    union {
                        char label[MAX_LABEL_LENGTH];
                        int num;
                        int counter;
                    }data_option;
                }data[80];
            }direct_options;
        }direct;
        struct{
            enum inst_type inst_type;
            struct{
                enum operand_address_type op_address;
                union{
                    int immed;
                    char label[MAX_LABEL_LENGTH];
                    char reg;
                    struct{
                        char label[MAX_LABEL_LENGTH];
                        union{
                            int num;
                            char label[MAX_LABEL_LENGTH];
                        }index_option;
                    }indexs;
                }opreand_option;
            }operands[2];
        }inst;
    }lex_tree_options;
};
struct lexer_tree get_lex_tree_from_line(const char *line);
#endif //COMPILERFINALPROJECT_LEXER_H
