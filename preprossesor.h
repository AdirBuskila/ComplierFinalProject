//
// Created by ניב on 05/04/2024.
//

#ifndef COMPLIERFINALPROJECT_PREPROSSESOR_H
#define COMPLIERFINALPROJECT_PREPROSSESOR_H
#include "globals.h"
#include "tabels.h"
#include "utils.h"
#define END_MACRO_DEC "endemcr"
#define MACRO_DEC "mcr"
typedef enum{
    macro_definition,
    end_macro_definition,
    macro_call,
    any_other_line,
}macro_states;
#endif //COMPLIERFINALPROJECT_PREPROSSESOR_H
