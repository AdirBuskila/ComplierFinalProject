<h1>FinalProject</h1>
<p>Final Project in the course System Programming Lab by the Open University of Israel. The purpose of this project is to build an assembler for a 16-instruction assembly language, for an imaginary 14-bit CPU with 14-bit & 256 words memory size. The authors of this project is Niv Harosh & Adir Buskila</p>

<h2>Running the Program</h2>
<p>To run the program, follow these steps:</p>
<p>1. Open a terminal window and navigate to the directory where the program files are located.</p>
<p>2. Run the <code>make</code> command to compile the program.</p>
<p>3. Run the <code>assembler</code> command followed by the name of the file(s) you want to assemble. The file(s) should be in the .as format and the input should be provided without the .as extension.</p>
<p>Here's an example command for assembling a single file named program:</p>
<code>$ assembler program</code>

<p>If the file(s) are syntaxly correct, the program will generate 4 output files in the same directory:</p>
<p>1. <code>.am</code> - Contains the macros from the original file spread, comment lines (starting with ;) deleted, and empty lines deleted.</p>
<p>2. <code>.ob</code> - Contains the binary code of the source code.</p>
<p>3. <code>.ext</code> - Contains the extern symbols in the source code.</p>
<p>4. <code>.ent</code> - Contains the entry symbols in the source code.</p>

<p>If the file(s) has errors the assembler won't produce any output file except the <code>.am file</code> and will print to the <code>stdout</code> all the errors it found.</p>

<h2>Directory Structure (Modules)</h2>
<p>1. <code>assembler</code> - Main function definition, argv & argc processing, single file processing.</p>
<p>2. <code>preprocessor</code> - The preprocessor of the assembler. </p>
<p>3. <code>first_pass</code> - The first pass of the assembler. </p>
<p>4. <code>second_pass</code> - The second pass of the assembler. </p>
<p>5. <code>globals.h</code> - Contains type and constant definitions. </p>
<p>6. <code>lexer.h</code> - Contains the definition of the Abstract Syntax Tree of a line in a source code. </p>
<p>7. <code>lexer</code> - Contains the implementation of the lexer of the assembler. </p>
<p>8. <code>code</code> - Functions related to the encoding of command line. </p>
<p>9. <code>directives</code> - Functions related to the encoding of directive line. </p>
<p>10. <code>tables</code> - Contains the implementation of Macros Table & Symbols Table. </p>
<p>11. <code>write_files</code> - Functions related to the creation of output files. </p>
<p>12. <code>utils</code> - Contains function for general use throughout the entire project. </p>
