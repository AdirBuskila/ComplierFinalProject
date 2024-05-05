//
// Created by ניב on 26/04/2024.
//

#include "lexer.h"
#include "utils.h"
#include "globals.h"

static enum operand_address_type examine_operand(char *op,struct lexer_tree *lex);
static void get_direct(const char *line,int *index,struct lexer_tree *lex);
static void get_direct_string(const char *line,int *index,struct lexer_tree *lex);
static void get_direct_data(const char *line,int *index,struct lexer_tree *lex);
static void get_ent_or_ext_label(const char *line,int *index,struct lexer_tree *lex);
static void get_opcode_inst(const char *line,int *index,struct lexer_tree *lex);
static void get_group_a_opcode(const char *line,int *index,struct lexer_tree *lex);
static void get_group_b_opcode(const char *line,int *index,struct lexer_tree *lex);
static void get_define(const char *line,int *index,struct lexer_tree *lex);
struct lexer_tree get_lex_tree_from_line(const char *line);

struct lexer_tree get_lex_tree_from_line(const char *line){
    struct lexer_tree lex={0};
    char *ptr;
    int i=0,index=0;
    char *tok=NULL;
    SKIP_WHITE_SPACE(line,i)
    ptr=(char*)(line+i);
    if(line[i] == '\0' || line[i] == '\n' || line[i] == EOF){
        lex.lex_tree_type=lex_empty;
        return lex;
    }
    SKIP_WHITE_SPACE(line,i)
    if((ptr= strchr(line,':'))!=NULL){
        index=ptr-(line+i);
        tok=(char*) malloc(index* sizeof(char)+1);
        strncpy(tok,line+i,index);
        tok[index]='\0';
        if(is_label(tok)){
            strcpy(lex.label,tok);
            free(tok);
            index+=(i+1);
        }
        else{
            lex.lex_tree_type=lex_error;
            strcpy(lex.error,"Invalid label.");
        }
    }
    else
        lex.label[0]='\0';
    SKIP_WHITE_SPACE(line,index)
    if(line[index]=='.'){
        if(strncmp(line+(*(&index)),"define",6)==0){
            get_define(line,&index,&lex);
            return lex;
        }
        else {
            get_direct(line, &index, &lex);
            SKIP_WHITE_SPACE(line, index)
            if(lex.lex_tree_options.direct.direct_type==lex_tree_string){
                get_direct_string(line,&index,&lex);
                return lex;
            }
            else if(lex.lex_tree_options.direct.direct_type==lex_tree_data){
                get_direct_data(line,&index,&lex);
                return lex;
            }
            else if(lex.lex_tree_options.direct.direct_type==lex_tree_extern||lex.lex_tree_options.direct.direct_type==lex_tree_entry){
                get_ent_or_ext_label(line,&index,&lex);
                return lex;
            }
        }
    }
    else{
        get_opcode_inst(line,&index,&lex);
        if(lex.lex_tree_type==lex_error)
            return lex;
        SKIP_WHITE_SPACE(line,index)
        /* Group A - using 2 parameters - mov,cmp,add,sub,lea*/
        if(lex.lex_tree_options.inst.inst_type==lex_inst_mov||lex.lex_tree_options.inst.inst_type==lex_inst_cmp
        ||lex.lex_tree_options.inst.inst_type==lex_inst_add||lex.lex_tree_options.inst.inst_type==lex_inst_sub
        ||lex.lex_tree_options.inst.inst_type==lex_inst_lea){
            get_group_a_opcode(line,&index,&lex);
            return lex;
        }
        if(lex.lex_tree_options.inst.inst_type==lex_inst_not||lex.lex_tree_options.inst.inst_type==lex_inst_clr
           ||lex.lex_tree_options.inst.inst_type==lex_inst_inc||lex.lex_tree_options.inst.inst_type==lex_inst_dec
           ||lex.lex_tree_options.inst.inst_type==lex_inst_jmp||lex.lex_tree_options.inst.inst_type==lex_inst_bne
           ||lex.lex_tree_options.inst.inst_type==lex_inst_red||lex.lex_tree_options.inst.inst_type==lex_inst_prn
           ||lex.lex_tree_options.inst.inst_type==lex_inst_jsr){
            get_group_b_opcode(line,&index,&lex);
            return lex;
        }
        if(lex.lex_tree_options.inst.inst_type==lex_inst_hlt||lex.lex_tree_options.inst.inst_type==lex_inst_rts){
            SKIP_WHITE_SPACE(line,index)
            if(line[index] == '\0' || line[index] == '\n' || line[index] == EOF){
                lex.lex_tree_type==lex_error;
                strcpy(lex.error,"There is trailing characters,Group c instruction do not have operands.");
                return lex;
            }
            return lex;
        }
    }
    return lex;
}
static void get_define(const char *line,int *index,struct lexer_tree *lex){
  SKIP_WHITE_SPACE(line,*index)
  if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
      lex->lex_tree_type=lex_error;
      strcpy(lex->error,"The define instruction must be follow by a string.");
      return;
  }
  const char *start=line+ *index;
    while (isalnum(line[*index]) || line[*index] == '_')
        (*index)++;
    int len=*index-(start-line);
    SKIP_WHITE_SPACE(line,*index)
    if (line[*index] != '=') {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "Missing '=' after identifier.");
        return;
    }
    (*index)++;
    SKIP_WHITE_SPACE(line, *index)
    char *endptr;
    long num = strtol(line + *index, &endptr, 10);
    if (line + *index == endptr) {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "No valid number found after '='.");
        return;
    }
    *index=endptr-line;
    SKIP_WHITE_SPACE(line,*index)
    if (line[*index] != '\0' && line[*index] != '\n') {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "Trailing characters after number.");
        return;
    }
    strncpy(lex->lex_tree_options.define.label,start,len);
    lex->lex_tree_options.define.label[len]='\0';
    lex->lex_tree_options.define.num=(int)num;
    lex->lex_tree_type=lex_define;
}
static void get_group_b_opcode(const char *line,int *index,struct lexer_tree *lex){
    int len;
    char *op;
    SKIP_WHITE_SPACE(line,*index)
    if(line[*index]==','){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"It started with a comma instead of a operand.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    op=(char*)calloc(len+1, sizeof(char));
    strncpy(op,line+(*index),len);
    op[len]='\0';
    (*index)+=len;
    SKIP_WHITE_SPACE(line,*index)
    if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"There are trailing characters after the end of the operand.");
        free(op);
        return;
    }
    lex->lex_tree_options.inst.operands[1].op_address= examine_operand(op,lex);
    if(lex->lex_tree_type==lex_error){
        free(op);
        return;
    }
    else{
        if (lex->lex_tree_options.inst.operands[1].op_address == lex_op_type_immed)
            lex->lex_tree_options.inst.operands[1].opreand_option.immed = atoi(op + 1);
        else if (lex->lex_tree_options.inst.operands[1].op_address == lex_op_type_label)
            strcpy(lex->lex_tree_options.inst.operands[1].opreand_option.label, op);
        else if (lex->lex_tree_options.inst.operands[1].op_address == lex_op_type_register)
            lex->lex_tree_options.inst.operands[1].opreand_option.reg = op[1];
        else {
            strcpy(lex->lex_tree_options.inst.operands[1].opreand_option.indexs.label, op);
            if (is_number(op))
                lex->lex_tree_options.inst.operands[1].opreand_option.indexs.index_option.num = atoi(op + 1);
            else
                strcpy(lex->lex_tree_options.inst.operands[1].opreand_option.indexs.index_option.label, op);
        }
    }
    free(op);
    (*index) += len;
    SKIP_WHITE_SPACE(line,*index)
    if (line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF) {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "There are trailing characters after the end of the second operand.");
        return;
    }
}
static void get_group_a_opcode(const char *line,int *index,struct lexer_tree *lex) {
    char *comma;
    int len;
    char *op;
    if ((comma = strchr(line + *index, ',')) == NULL) {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "Group a must have two operand that are separated by a comma.");
        return;
    }
    SKIP_WHITE_SPACE(line, *index)
    if (line[*index] == ',') {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "It started with a comma instead of a operand.");
        return;
    }
    for (len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                  line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    op = (char *) calloc(len + 1, sizeof(char));
    strncpy(op, line + (*index), len);
    op[len] = '\0';
    lex->lex_tree_options.inst.operands[0].op_address = examine_operand(op, lex);
    if (lex->lex_tree_type == lex_error) {
        strcpy(lex->error, "The first operand is illegal.");
        free(op);
        return;
    } else {
        if (lex->lex_tree_options.inst.operands[0].op_address == lex_op_type_immed)
            lex->lex_tree_options.inst.operands[0].opreand_option.immed = atoi(op + 1);
        else if (lex->lex_tree_options.inst.operands[0].op_address == lex_op_type_label)
            strcpy(lex->lex_tree_options.inst.operands[0].opreand_option.label, op);
        else if (lex->lex_tree_options.inst.operands[0].op_address == lex_op_type_register)
            lex->lex_tree_options.inst.operands[0].opreand_option.reg = op[1];
        else {
            strcpy(lex->lex_tree_options.inst.operands[0].opreand_option.indexs.label, op);
            if (is_number(op))
                lex->lex_tree_options.inst.operands[0].opreand_option.indexs.index_option.num = atoi(op + 1);
            else
                strcpy(lex->lex_tree_options.inst.operands[0].opreand_option.indexs.index_option.label, op);
        }
    }

    free(op);
    (*index) += len;
    SKIP_WHITE_SPACE(line,*index)
    (*index)++;
    SKIP_WHITE_SPACE(line,*index)
    if (line[*index] == ',') {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "Consecutive commas detected.");
        return;
    }
    for (len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                  line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    op = (char *) calloc(len + 1, sizeof(char));
    strncpy(op, line + (*index), len);
    op[len] = '\0';
    lex->lex_tree_options.inst.operands[1].op_address = examine_operand(op, lex);
    if (lex->lex_tree_type == lex_error) {
        strcpy(lex->error, "The second operand is illegal.");
        free(op);
        return;
    } else {
        if (lex->lex_tree_options.inst.operands[1].op_address == lex_op_type_immed)
            lex->lex_tree_options.inst.operands[1].opreand_option.immed = atoi(op + 1);
        else if (lex->lex_tree_options.inst.operands[1].op_address== lex_op_type_label)
            strcpy(lex->lex_tree_options.inst.operands[1].opreand_option.label, op);
        else if (lex->lex_tree_options.inst.operands[1].op_address == lex_op_type_register)
            lex->lex_tree_options.inst.operands[1].opreand_option.reg = op[1];
        else {
            strcpy(lex->lex_tree_options.inst.operands[1].opreand_option.indexs.label, op);
            if (is_number(op))
                lex->lex_tree_options.inst.operands[1].opreand_option.indexs.index_option.num = atoi(op + 1);
            else
                strcpy(lex->lex_tree_options.inst.operands[1].opreand_option.indexs.index_option.label, op);
        }
    }
    free(op);
    (*index) += len;
    SKIP_WHITE_SPACE(line,*index)
    if (line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF) {
        lex->lex_tree_type = lex_error;
        strcpy(lex->error, "There are trailing characters after the end of the second operand.");
        return;
    }
}


static void get_opcode_inst(const char *line,int *index,struct lexer_tree *lex){
    int len;
    char *potential_cmd=NULL;
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    struct cmd{char *inst_name; enum inst_type opcode;};
    struct cmd cmd_list[]={
            {"mov", lex_inst_mov}, {"cmp", lex_inst_cmp}, {"add", lex_inst_add},
            {"sub", lex_inst_sub}, {"not", lex_inst_not}, {"clr", lex_inst_clr},
            {"lea", lex_inst_lea}, {"inc", lex_inst_inc}, {"dec", lex_inst_dec},
            {"jmp", lex_inst_jmp}, {"bne", lex_inst_bne}, {"red", lex_inst_red},
            {"prn", lex_inst_prn}, {"jsr", lex_inst_jsr},
            {"rts", lex_inst_rts}, {"hlt", lex_inst_hlt}
    };
    potential_cmd= (char*)malloc(len* sizeof(char)+1);
    strncpy(potential_cmd,line+(*index),len);
    potential_cmd[len]='\0';
    (*index)+=len;
    lex->lex_tree_type=lex_inst;
    {
        struct cmd *ptr = NULL;
        int i;
        for (ptr = cmd_list, i = 0; i < OPCODE_COUNT; i++, ptr++) {
            if(strcmp(potential_cmd,ptr->inst_name)==0){
                lex->lex_tree_options.inst.inst_type=ptr->opcode;
                return;
            }
        }
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"Command doesn't exist. ");
    }
}
static void get_ent_or_ext_label(const char *line,int *index,struct lexer_tree *lex){
    char *tok=NULL;
    int len=0;
    SKIP_WHITE_SPACE(line,*index);
    if(lex->lex_tree_options.direct.direct_options.label[0]!='\0'){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,".entry/.extern instruction can't be a label definition");
        return;
    }
    if(line[*index]=='\0'||line[*index]=='\n'||line[*index]==EOF){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,".entry / .extern must be followed by a label");
        return;
    }

    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    tok=(char*)malloc((len+1)* sizeof(char));
    strncpy(tok,line+(*index),len);
    tok[len]='\0';
    (*index)+=len;
    if(!is_label(tok)){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"Invalid label.");
        free(tok);
        return;
    }
    strcpy(lex->lex_tree_options.direct.direct_options.label,tok);
    free(tok);
    SKIP_WHITE_SPACE(line,*index);
    if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"There are trailing characters after the end of the label.");
        return;
    }

}
static void get_direct_data(const char *line,int *index,struct  lexer_tree *lex){
    char *end_ptr;
    int val;
    int counter=0;
    do{
        SKIP_WHITE_SPACE(line,*index)
        if(line[*index]==','){
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"Start with a comma instead of a integer.");
            return;
        }
        else if(line[*index]=='\0'||line[*index]=='\n'||line[*index]==EOF){
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"The .data instruction must have at least one integer or a string.");
            return;
        }
        val= strtol(line+(*index),&end_ptr,10);
        if(end_ptr==line+(*index)){
            strcpy(lex->lex_tree_options.direct.direct_options.data[counter].data_option.label,end_ptr);
            counter++;
        }
        if(val>MAX_INT_VALUE||val<MIN_INT_VALUE){
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"The integer is out of range.");
            return;
        }
        lex->lex_tree_options.direct.direct_options.data[counter].data_option.num=val;
        lex->lex_tree_options.direct.direct_options.data[counter].data_option.counter++;
        counter++;
        (*index)+= end_ptr-(line+(*index));
        SKIP_WHITE_SPACE(line,*index);
        if(line[*index]=='\0'||line[*index]=='\n'||line[*index]==EOF){
            return;
        }
        else{
            if(line[*index]!=','){
                lex->lex_tree_type=lex_error;
                strcpy(lex->error,"Invalid character,must be ','.");
                return;
            }
            (*index)++;
            SKIP_WHITE_SPACE(line,*index);
            if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
                lex->lex_tree_type=lex_error;
                strcpy(lex->error,"trailing comma detected.");
            }
            else if(line[*index]==','){
                lex->lex_tree_type=lex_error;
                strcpy(lex->error,"Consecutive commas detected.");
                return;
            }
        }
    }while(*end_ptr!='\n'&&*end_ptr!='\0'&&*end_ptr!=EOF);
}
static void get_direct_string(const char *line,int *index,struct lexer_tree *lex){
    char *end_str;
    SKIP_WHITE_SPACE(line,*index)
    if(line[*index]=='\0'||line[*index]=='\n'||line[*index]==EOF){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"The .string instruction must have a string.");
        return;
    }
    if(line[*index]!='"'){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"String must start with quotes.");
        return;
    }
    (*index)++;
    end_str= strchr(line+(*index),'"');
    if(end_str==NULL){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"String must end with quotes.");
        return;
    }
    else{
        int length=end_str-(line+(*index));
        strncpy(lex->lex_tree_options.direct.direct_options.string,line+(*index),length);
        lex->lex_tree_options.direct.direct_options.string[length]='\0';
        (*index)+=length+1;
    }
    SKIP_WHITE_SPACE(line,*index)
    if(line[*index]!='\0'&&line[*index]!='\n'&&line[*index]!=EOF){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"There is characters after the end of the string.");
        return;
    }

}
static void get_direct(const char *line,int *index,struct lexer_tree *lex){
    (*index)++;
    if(strncmp(line+(*index),"string",6)==0){
        lex->lex_tree_type=lex_direct;
        lex->lex_tree_options.direct.direct_type=lex_tree_string;
        (*index)+=6;
    }
    else if(strncmp(line+(*index),"data",4)==0){
        lex->lex_tree_type=lex_direct;
        lex->lex_tree_options.direct.direct_type=lex_tree_data;
        (*index)+=4;
    }
    else if(strncmp(line+(*index),"entry",5)==0){
        lex->lex_tree_type=lex_direct;
        lex->lex_tree_options.direct.direct_type=lex_tree_entry;
        (*index)+=5;
    }
    else if(strncmp(line+(*index),"extern",6)==0){
        lex->lex_tree_type=lex_direct;
        lex->lex_tree_options.direct.direct_type=lex_tree_extern;
        (*index)+=6;
    }
    else{
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"Invalid instruction.");
        return;
    }
}
static enum operand_address_type examine_operand(char *op,struct lexer_tree *lex){
    char *endptr;
    long val;
    char *val_str;

    char *temp;
    if(strlen(op)==0){
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"Missing operand.");
        return lex_op_type_error;
    }
    if(op[0]=='#'|| is_number(op)){
        if(is_number(op)){
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"Immediate operand need to start with '#'.");
            return lex_op_type_error;
        }
        val_str=&op[1];
        temp= strchr(val_str,'#');
        if(temp!=NULL){
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"Multiple '#' symbols before immediate operand");
            return lex_op_type_error;
        }
        if(*val_str=='\0'){
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"Missing integer or a define that was declared after '#' sign. ");
            return lex_op_type_error;
        }
        val= strtol(val_str,&endptr,10);
        if(*endptr!='\0'){
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"Invalid integer.");
            return lex_op_type_error;
        }
        if(val>=-(1<<11)&&val<(1<<11)){
            return lex_op_type_immed;
        }
        else if(val<-(1<<11)||val>(1<<11)) {
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"Out of range integer.");
            return lex_op_type_error;
        }
        else{
            /* is a decleard define*/
            return lex_op_type_immed;
        }
    }
    else if(strlen(op)==2&&op[0]=='r'&& isdigit(op[1])){
        int register_num=op[1]-'0';
        if(register_num>=0&&register_num<=7)
            return lex_op_type_register;
        else{
            lex->lex_tree_type=lex_error;
            strcpy(lex->error,"Invalid register number.");
            return lex_op_type_error;
        }
    }
    else if(is_label(op)){
        return lex_op_type_label;
    }
    else if(is_array(op)){
        return lex_op_type_index_const;
    }
    else{
        lex->lex_tree_type=lex_error;
        strcpy(lex->error,"Illegal opereand");
        return lex_op_type_error;
    }

}