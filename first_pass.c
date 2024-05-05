//
// Created by ניב on 15/04/2024.
//

#include "first_pass.h"
#include "globals.h"
#include "lexer.h"
#include "initialize.h"
#include "utils.h"
#include "tabels.h"
bool first_pass(line_description line,sym_table *table,long *ic,long *dc, data_word *data_img,machine_word *code_img,struct lexer_tree *lex){
    int i=0;
    if(lex->label[0]!='\0'){
        if(find_sym_content(table,lex->label)!=NULL){
            if(find_sym_content(table,lex->label)->sym_t==extern_sym){
                print_error_msg(&line,"Extern label can't be define in the same file.");
                return FALSE;
            }
            else{
                print_error_msg(&line,"Label is already defined.");
                return FALSE;
            }
        }
        if(lex->lex_tree_type==lex_direct){
            if(lex->lex_tree_options.direct.direct_type==lex_tree_string||lex->lex_tree_options.direct.direct_type==lex_tree_data){
                if(lex->label&& find_sym_content(table,lex->label)==NULL)
                    insert_symbol_content(table,lex->label,*dc,data_sym);
                if(lex->lex_tree_options.direct.direct_type==lex_tree_string){
                    insert_string(lex->lex_tree_options.direct.direct_options.string,data_img,dc);
                }
                else{
                    for(i=0;i<lex->lex_tree_options.direct.direct_options.data[i].data_option.counter;i++){
                        insert_data_number(lex->lex_tree_options.direct.direct_options.data[i].data_option.num,data_img,dc);
                    }
                }


            }
            else if(lex->lex_tree_options.direct.direct_type==lex_tree_entry||lex->lex_tree_options.direct.direct_type==lex_tree_extern){
                print_error_msg(&line,".entry or .extern in label definition are useless.");
                return FALSE;
            }
        }
        if(lex->lex_tree_type==lex_inst){
            insert_symbol_content(table,lex->label,*ic,code_sym);
            return insert_opcode(&line,lex,code_img,ic,table);
        }
    }
    else{
        if(lex->lex_tree_type==lex_direct){
            if(lex->lex_tree_options.direct.direct_type==lex_tree_string||
            lex->lex_tree_options.direct.direct_type==lex_tree_data){
                if(lex->lex_tree_options.direct.direct_type==lex_tree_string){
                    insert_string(lex->lex_tree_options.direct.direct_options.string,data_img,dc);
                }
                else{
                    for(i=0;i<lex->lex_tree_options.direct.direct_options.data[i].data_option.counter;i++){
                        insert_data_number(lex->lex_tree_options.direct.direct_options.data[i].data_option.num,data_img,dc);
                    }
                }
            }
            else if(lex->lex_tree_options.direct.direct_type==lex_tree_extern){
                insert_symbol_content(table,lex->lex_tree_options.direct.direct_options.label,0,extern_sym);
                return TRUE;
            }
            else if(lex->lex_tree_options.direct.direct_type==lex_tree_entry){
                return TRUE;
            }
        }
        else if(lex->lex_tree_type==lex_inst)
            return insert_opcode(&line,lex,code_img,ic,table);
    }
    return TRUE;
}