cmake_minimum_required(VERSION 3.27)
project(CompilerFinalProject C)

set(CMAKE_C_STANDARD 90)
add_executable(assembler
        tabels.c
        preprossesor.c
        utils.c
        asm_files.c
        initialize.c
        lexer.c
        first_pass.c
        second_pass.c
        assembler.c
)