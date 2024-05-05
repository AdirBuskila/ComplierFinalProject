//
// Created by ניב on 15/04/2024.
//
#include <stdio.h>
#include "second_pass.h"
#include "globals.h"
#include "lexer.h"
#include "tabels.h"
#include "utils.h"

bool second_pass_process(struct lexer_tree lex,sym_table *table,line_description line){
    sym_table_content *con;

    if(lex.lex_tree_type==lex_inst){
        if(lex.lex_tree_options.inst.inst_type==lex_inst_mov||lex.lex_tree_options.inst.inst_type==lex_inst_cmp
           ||lex.lex_tree_options.inst.inst_type==lex_inst_add||lex.lex_tree_options.inst.inst_type==lex_inst_sub
           ||lex.lex_tree_options.inst.inst_type==lex_inst_lea){
            if(lex.lex_tree_options.inst.operands[0].op_address==lex_op_type_label) {
                if (find_sym_content(table, lex.lex_tree_options.inst.operands[0].opreand_option.label) == NULL) {
                    print_error_msg(&line, "The label %s is not defined",
                                    lex.lex_tree_options.inst.operands[0].opreand_option.label);
                    return FALSE;
                }
            }
                else if(lex.lex_tree_options.inst.operands[1].op_address==lex_op_type_label){
                    if(find_sym_content(table,lex.lex_tree_options.inst.operands[1].opreand_option.label)==NULL){
                        print_error_msg(&line,"The label %s is not defined",lex.lex_tree_options.inst.operands[1].opreand_option.label);
                        return FALSE;
                    }
                }
        }
        else if(lex.lex_tree_options.inst.inst_type==lex_inst_not||lex.lex_tree_options.inst.inst_type==lex_inst_clr
                ||lex.lex_tree_options.inst.inst_type==lex_inst_inc||lex.lex_tree_options.inst.inst_type==lex_inst_dec
                ||lex.lex_tree_options.inst.inst_type==lex_inst_jmp||lex.lex_tree_options.inst.inst_type==lex_inst_bne
                ||lex.lex_tree_options.inst.inst_type==lex_inst_red||lex.lex_tree_options.inst.inst_type==lex_inst_prn
                ||lex.lex_tree_options.inst.inst_type==lex_inst_jsr){
            if(lex.lex_tree_options.inst.operands[1].op_address==lex_op_type_label){
                if(find_sym_content(table,lex.lex_tree_options.inst.operands[1].opreand_option.label)==NULL){
                    print_error_msg(&line,"The label %s is not defined",lex.lex_tree_options.inst.operands[1].opreand_option.label);
                    return FALSE;
                }
            }
        }
        else {
            return TRUE;
        }
    }
    if(lex.lex_tree_options.direct.direct_type==lex_tree_entry){
        con= find_sym_content(table,lex.lex_tree_options.direct.direct_options.label);
        if(con==NULL){
            print_error_msg(&line,"The label %s is not defined",lex.lex_tree_options.direct.direct_options.label);
            return FALSE;
        }
        else{
            con->sym_t=entry_sym;
            return TRUE;
        }
    }
    else if(lex.lex_tree_options.direct.direct_type==lex_tree_extern){
        con= find_sym_content(table,lex.lex_tree_options.direct.direct_options.label);
        if(con==NULL){
            print_error_msg(&line,"The label %s is not defined",lex.lex_tree_options.direct.direct_options.label);
            return FALSE;
        }
        else{
            con->sym_t=extern_sym;
            return TRUE;
        }
    }
    return TRUE;
}
void init_second_pass(sym_table *table,machine_word *code_img,long ic){
    int i;
    for(i=0;i<ic;i++){
        if(code_img[i].label_name!=NULL){
            sym_table_content *con= find_sym_content(table,code_img[i].label_name);
            if(con==NULL)
                continue;
            if(con->sym_t==extern_sym){
                code_img[i].type_word.imm_or_dir_word=(immd_or_direct_word*) calloc(1, sizeof(immd_or_direct_word));
                code_img[i].type_word.imm_or_dir_word->are_bits=1;
                code_img->type_word.imm_or_dir_word->opreand_bits=0;
            }
            else{
                code_img[i].type_word.imm_or_dir_word=(immd_or_direct_word*) calloc(1, sizeof(immd_or_direct_word));
                code_img[i].type_word.imm_or_dir_word->are_bits=2;
                code_img[i].type_word.imm_or_dir_word->opreand_bits=(con->symbol_address)+DATA_SEGMENT_START_ADDRESS;
            }

        }

    }
}