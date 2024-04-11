cmake_minimum_required(VERSION 3.27)
project(CompilerFinalProject C)

set(CMAKE_C_STANDARD 90)
add_executable(CompilerFinalProject tabels.c
        tabels.h
        preprossesor.c
        preprossesor.h
        utils.c
        utils.h
        globals.h
        asm_files.c
        asm_files.h

)