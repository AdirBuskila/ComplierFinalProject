//
// Created by ניב on 21/04/2024.
//

#include "initialize.h"
#include "globals.h"
#include "lexer.h"
static void init_immd_word(int opreand,long *ic,machine_word code_im[]);
static void init_first_word(enum inst_type cmd,enum operand_address_type first_op_type,enum operand_address_type second_op_type,long *ic,machine_word code_img[]);
static void init_label_word(char label[],long *ic,machine_word code_img[]);
static void init_register_word(char first_reg,char second_reg,long *ic,machine_word code_img[]);
static immd_or_direct_word init_12_bit(int value);
static data_word insert_char(char c);
static data_word init_14_bits(short int value);

bool insert_opcode(line_description *line,struct lexer_tree *lex,machine_word *code_img,long *ic,sym_table *table){
    if(lex->lex_tree_options.inst.inst_type==lex_inst_mov||lex->lex_tree_options.inst.inst_type==lex_inst_cmp
       ||lex->lex_tree_options.inst.inst_type==lex_inst_add||lex->lex_tree_options.inst.inst_type==lex_inst_sub
       ||lex->lex_tree_options.inst.inst_type==lex_inst_lea) {
        /* lea opcode */
        if (lex->lex_tree_options.inst.inst_type == lex_inst_lea) {
            if (lex->lex_tree_options.inst.operands[0].op_address == lex_op_type_immed ||
                lex->lex_tree_options.inst.operands[0].op_address == lex_op_type_register) {
                print_error_msg(line, "Illegal source operand type for the  the instruction.");
                return FALSE;
            } else if (lex->lex_tree_options.inst.operands[1].op_address == lex_op_type_immed) {
                print_error_msg(line, "Illegal destination operand type for the  the instruction. ");
                return FALSE;
            }
        }
        if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_immed &&
        lex->lex_tree_options.inst.inst_type!=lex_inst_cmp){
            print_error_msg(line,"Illegal operand address for destination operand for the instruction.");
        }
        init_first_word(lex->lex_tree_options.inst.inst_type,lex->lex_tree_options.inst.operands[0].op_address
                        ,lex->lex_tree_options.inst.operands[1].op_address,ic,code_img);

        if(lex->lex_tree_options.inst.operands[0].op_address==lex_op_type_register&&
                lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_register){
            init_register_word(lex->lex_tree_options.inst.operands[0].opreand_option.reg,
                               lex->lex_tree_options.inst.operands[1].opreand_option.reg,ic,code_img);
            return TRUE;
        }
        if(lex->lex_tree_options.inst.operands[0].op_address==lex_op_type_immed){
            init_immd_word(lex->lex_tree_options.inst.operands[0].opreand_option.immed,ic,code_img);
        }
        else if(lex->lex_tree_options.inst.operands[0].op_address==lex_op_type_label){
            init_label_word(lex->lex_tree_options.inst.operands[0].opreand_option.label,ic,code_img);
        }
        else if(lex->lex_tree_options.inst.operands[0].op_address==lex_op_type_register){
            init_register_word(lex->lex_tree_options.inst.operands[0].opreand_option.reg,-1,ic,code_img);
        }
        if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_immed){
            init_immd_word(lex->lex_tree_options.inst.operands[1].opreand_option.immed,ic,code_img);
            return TRUE;
    }
        else if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_label) {
            init_label_word(lex->lex_tree_options.inst.operands[1].opreand_option.label, ic, code_img);
            return TRUE;
        }
        else if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_register){
            init_register_word(-1,lex->lex_tree_options.inst.operands[1].opreand_option.reg,ic,code_img);
            return TRUE;
        }
    }

    if(lex->lex_tree_options.inst.inst_type==lex_inst_not||lex->lex_tree_options.inst.inst_type==lex_inst_clr
       ||lex->lex_tree_options.inst.inst_type==lex_inst_inc||lex->lex_tree_options.inst.inst_type==lex_inst_dec
       ||lex->lex_tree_options.inst.inst_type==lex_inst_jmp||lex->lex_tree_options.inst.inst_type==lex_inst_bne
       ||lex->lex_tree_options.inst.inst_type==lex_inst_red||lex->lex_tree_options.inst.inst_type==lex_inst_prn
       ||lex->lex_tree_options.inst.inst_type==lex_inst_jsr){

        if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_immed&&
                lex->lex_tree_options.inst.inst_type!=lex_inst_prn){
            print_error_msg(line,"Illegal for the operand addressing type.");
            return FALSE;
        }
        init_first_word(lex->lex_tree_options.inst.inst_type,0,lex->lex_tree_options.inst.operands[1].op_address,ic,code_img);
        if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_immed){
            init_immd_word(lex->lex_tree_options.inst.operands[1].opreand_option.immed,ic,code_img);
            return TRUE;
        }
        else if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_label) {
            init_label_word(lex->lex_tree_options.inst.operands[1].opreand_option.label, ic, code_img);
            return TRUE;
        }
        else if(lex->lex_tree_options.inst.operands[1].op_address==lex_op_type_register){
            init_register_word(-1,lex->lex_tree_options.inst.operands[1].opreand_option.reg,ic,code_img);
            return TRUE;
        }
    }
    if((lex->lex_tree_options.inst.inst_type==lex_inst_hlt||lex->lex_tree_options.inst.inst_type==lex_inst_rts)){
        init_first_word(lex->lex_tree_options.inst.inst_type,0,0,ic,code_img);
        return TRUE;
    }
    return TRUE;
}
static immd_or_direct_word init_12_bit(int value){
    immd_or_direct_word result;

    if (value < 0) {
        // Convert negative value to two's complement for 12-bit
        unsigned int abs_value = abs(value);
        unsigned int complement = (~abs_value) + 1;

        // Use only the least 12 bits and set the sign bit (most significant bit in 12 bits)
        result.opreand_bits = (1 << 11) | (complement & 0x07FF);  // Ensure only 12 bits are used
    } else {
        // Directly mask the value to fit 12 bits for positive numbers
        result.opreand_bits = value & 0x0FFF;
    }

    return result;
}
static void init_immd_word(int opreand,long *ic,machine_word code_im[]){
    machine_word new_word={0};
    new_word.label_name=NULL;
    new_word.type_word.imm_or_dir_word= (immd_or_direct_word*) calloc(1, sizeof(immd_or_direct_word));
    *(new_word.type_word.imm_or_dir_word)= init_12_bit(opreand);
    new_word.type_word.imm_or_dir_word->are_bits=0;
    code_im[(*ic)++]=new_word;
}
static void init_label_word(char label[],long *ic,machine_word code_img[]){
    if (label == NULL || ic == NULL || code_img == NULL) {
        return; // Guard against null pointer dereferences
    }

    // Initialize the word
    machine_word new_word = {0}; // Zero out the struct
   new_word.label_name = strdup(label); // Duplicate the label
    new_word.type_word.first_word = NULL; // Explicitly set to NULL

    if (new_word.label_name == NULL) {
        fprintf(stderr, "Failed to allocate memory for label.\n");
        return; // Allocation failed, exit the function
    }

    // Add the encoded word to the code image and increment the instruction counter
    code_img[(*ic)++] = new_word;

}
static void init_register_word(char first_reg,char second_reg,long *ic,machine_word code_img[]){
    machine_word new_word;
    new_word.label_name=NULL;
    new_word.type_word.reg_word=(reg_word*) calloc(1,sizeof(reg_word));
    if(first_reg>=0)
        new_word.type_word.reg_word->regs_src_bits=first_reg-'0';
    if(second_reg>=0)
        new_word.type_word.reg_word->reg_dest_bits=second_reg-'0';
    code_img[(*ic)++]=new_word;
}
static void init_first_word(enum inst_type cmd,enum operand_address_type first_op_type,enum operand_address_type second_op_type,long *ic,machine_word code_img[]){
    machine_word new_word;
    new_word.label_name=NULL;
    new_word.type_word.first_word=(first_word*) calloc(1, sizeof(first_word));
    new_word.type_word.first_word->opcode_bits=cmd;
    new_word.type_word.first_word->src_opcode_add_bits=first_op_type;
    new_word.type_word.first_word->dest_opcode_add_bits=second_op_type;
    new_word.type_word.first_word->are_bits=0;
    new_word.type_word.first_word->not_used_bits=0;
    code_img[(*ic)++]=new_word;
}
static data_word insert_char(char c){
    data_word new_word={0};
    new_word.data=(unsigned int)(unsigned char)c;
    return new_word;
}
static data_word init_14_bits(short int value){
    data_word result = {0}; // Initialize the result to zero.

    if (value < 0) {
        // Convert negative value to two's complement
        // Since we're dealing with 14 bits, mask with 0x3FFF after converting
        result.data = (unsigned int)((~(-value) + 1) & 0x3FFF);
        result.data |= (1 << 13); // Ensure the sign bit (14th bit) is set
    } else {
        // Mask the value to ensure it's within 14 bits
        result.data = (unsigned int)(value & 0x3FFF);
    }

    return result;
}
void insert_string(char *str,data_word *data_img,long *dc){
     int i;
     for(i=0;str[i]!='\0';i++)
         data_img[(*dc)++]= insert_char(str[i]);
     data_img[(*dc)++]= insert_char('\0');
}
void insert_data_number(int num,data_word *data_im,long *dc){
   data_im[(*dc)++]= init_14_bits(num);
}