//
// Created by ניב on 29/04/2024.
//
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "preprossesor.h"
#include "asm_files.h"
#include "first_pass.h"
#include "second_pass.h"
#include "tabels.h"
#include "utils.h"

bool file_proseccer(char *file_name);

int main(int argc,char *argv[]){
    int i;
    if(argc<2){
        fprintf(stderr,"Usage: %s file1 file2 file3...",argv[0]);
        exit(EXIT_FAILURE);
    }
    for(i=1;i<argc;i++){
        if(strlen(argv[i])>MAX_LINE_LENGTH){
            fprintf(stderr,"The file name %s is too long (max %d characters)\n",argv[i],MAX_LINE_LENGTH);
            continue;
        }
        file_proseccer(argv[i]);
    }
    return 0;
}

bool file_proseccer(char *file_name){
    long ic=0,dc=0;
    data_word data_img[MEMORY_SIZE]={0};
    machine_word code_img[MEMORY_SIZE]={0};
    char *am_file_name=NULL;
    char temp[MAX_LINE_LENGTH+1];
    line_description curr_line;
    sym_table *table=NULL;
    FILE *file=NULL;
    bool success=TRUE,ext_sym=FALSE,ent_sym=FALSE;
    char temp_char;
    struct lexer_tree line_struct;

    if(!preprocess_file(file_name)){
        printf("The macros didn't spread correctly into %s.as\n",file_name);
        return FALSE;
    }
    am_file_name= str_allocate_cat(file_name,".am");
    file= fopen(am_file_name,"r");
    if(file==NULL){
        printf("The file %s couldn't be open.\n",am_file_name);
        free(am_file_name);
        return FALSE;
    }
    table=create_sym_table();
    curr_line.file_name=am_file_name;
    curr_line.content=temp;
    for(curr_line.line_number=1; fgets(temp,MAX_LINE_LENGTH+1,file)!=NULL;curr_line.line_number++){
        if(strchr(temp,'\n')==NULL&&!feof(file)){
            print_error_msg(&curr_line,"The line is to big 80 characters is the max.");
            success=FALSE;
            do{
                temp_char= fgetc(file);

            }while(temp_char!='\n'&&temp_char!=EOF);
        }
        else{
            line_struct= get_lex_tree_from_line(temp);
            if(line_struct.lex_tree_type==lex_error){
                print_error_msg(&curr_line,line_struct.error);
                success=FALSE;
            }
            else{
                if(line_struct.lex_tree_type==lex_direct){
                    if(line_struct.lex_tree_options.direct.direct_type==lex_tree_entry)
                        ent_sym=TRUE;
                    else if(line_struct.lex_tree_options.direct.direct_type==lex_tree_extern)
                        ext_sym=TRUE;
                }
                success= first_pass(curr_line,table,&ic,&dc,data_img,code_img,&line_struct);
            }
        }
    }
    update_symbol_table(table,ic);
    rewind(file);
    if(success){
        for(curr_line.line_number=1; fgets(temp,MAX_LINE_LENGTH+1,file)!=NULL;curr_line.line_number++){
            line_struct= get_lex_tree_from_line(temp);
            success= second_pass_process(line_struct,table,curr_line);
        }
        init_second_pass(table,code_img,ic);
    }
    if(ic+dc>MEMORY_SIZE-DATA_SEGMENT_START_ADDRESS){
        print_error_msg(&curr_line,"The memory size is too small.");
        success=FALSE;
    }
    if(success){
        write_obj_file(file_name,code_img,ic,data_img,dc);
        if(ent_sym)
            write_entry_file(file_name,table);
        if(ext_sym)
            write_extern_file(file_name,table,code_img,ic);
    }
    free(am_file_name);
    fclose(file);
    destroy_symbol_table(table);
    free_code_image(code_img,ic);
    return TRUE;
}