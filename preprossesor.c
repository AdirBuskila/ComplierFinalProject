//
// Created by ניב on 05/04/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "preprossesor.h"
#include "globals.h"
#include "utils.h"
#include "tabels.h"


bool preprocess_file(char *filename) {
    bool macro_declration;
    int num_of_words_in_line = 0,index=0;
    char current_line[MAX_LINE_LENGTH + 2];
    char **splitted_line;
    char *macro_name=NULL;
    macro_table *table = create_macro_table();
    char *am_filename = str_allocate_cat(filename, ".am");
    char *origin_filename = str_allocate_cat(filename, ".as");
    FILE *origin_file_des = fopen(origin_filename, "r");
    FILE *am_file_des ;

    if(origin_file_des==NULL){
        printf("The file %s couldn't open\n",origin_filename);
        free(am_filename);
        free_macro_table(table);
        free(origin_filename);
        return FALSE;
    }
    am_file_des= fopen(am_filename,"w+");
    if(am_file_des==NULL){
        printf("The file %s couldn't open\n",am_filename);
        free(am_filename);
        free(origin_filename);
        fclose(origin_file_des);
        fclose(am_file_des);
        free_macro_table(table);
        return FALSE;
    }
    while(fgets(current_line,MAX_LINE_LENGTH+1,origin_file_des)){
        SKIP_WHITE_SPACE(current_line,index)
        if(current_line[index]=='\n'||current_line[index]==EOF||current_line[index]==';'||current_line[index]=='\0')
            continue;
        splitted_line= split_string(current_line+index," ",&num_of_words_in_line);
        if(get_lines(table,splitted_line[0])!=NULL)
            fputs(get_lines(table,splitted_line[0]),am_file_des);
        else if(strcmp(splitted_line[0],MACRO_DEC)==0){
            macro_declration=TRUE;
            macro_name=(char*) calloc(MAX_MACRO_NAME_LENGTH, sizeof(char ));
            strcpy(macro_name,splitted_line[1]);
            insert_macro_content(table,macro_name,"");
        }
        else if(strcmp(splitted_line[0],END_MACRO_DEC)==0){
            macro_declration=FALSE;
        }
        else if(macro_declration==TRUE){
            char *new_macro= str_allocate_cat(get_lines(table,macro_name),current_line);
            insert_macro_content(table,macro_name,new_macro);
            free(new_macro);
        }
        else{
            fputs(current_line,am_file_des);
        }
        free_split_string(splitted_line,num_of_words_in_line);
    }

    free(am_filename);
    free(origin_filename);
    fclose(origin_file_des);
    fclose(am_file_des);
    free_macro_table(table);
    if(macro_name!=NULL)
        free(macro_name);
    return TRUE;
}