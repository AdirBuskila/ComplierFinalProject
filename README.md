Assembler Project
This assembler is a collaborative project developed by Adir Buskila and Niv Harosh for the Systems Programming Laboratory course. Our assembler translates assembly language code, defined specifically for this project, into machine code, mimicking fundamental system software's operations.

Project Overview
The project's primary goal is to provide hands-on experience with the intricacies of system software development, particularly focusing on the assembly process. Written entirely in C, this assembler supports a predefined assembly language, adhering to strict compilation flags to ensure high-quality, standard-compliant code.

Features
Two-Pass Assembly Process: Implements a two-pass strategy for symbol resolution and machine code generation.
Macro Preprocessing: Supports macro definitions and expansions, allowing for more concise and reusable assembly code.
Comprehensive Error Handling: Detects and reports syntax errors, undefined symbols, and invalid instructions with detailed messages.
Symbol and Macro Tables: Manages symbols and macros efficiently, supporting external and internal references.
Output File Generation: Produces machine code, symbol tables, and error logs as separate files, facilitating further linking and loading processes.
Getting Started
Prerequisites
GCC compiler
GNU Make
Basic Unix/Linux command line knowledge
Compilation
To compile the assembler, navigate to the project directory and run the following command:

bash
Copy code
make
This command utilizes the Makefile to compile the source code with the appropriate gcc flags, generating an executable.

Usage
To run the assembler on assembly source files:

bash
Copy code
./assembler filename1 filename2 ...
Replace filename1 filename2 ... with the names of your assembly language source files, excluding the .as extension.

Output Files
The assembler generates the following files for each input source:

.ob (Object File): Contains the generated machine code.
.ent (Entries File): Details symbols declared as entry points.
.ext (Externals File): Lists references to external symbols.
.am (Preprocessed Source File): Shows the source file post-macro expansion.
Contributors
This project was developed by:

Adir Buskila
Niv Harosh
We welcome contributions and suggestions to improve the assembler. Feel free to fork the repository and submit pull requests.

License
This project is licensed under the MIT License - see the LICENSE.md file for details.
