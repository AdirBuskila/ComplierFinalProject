<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Assembler Project README</title>
    <style>
        body { font-family: Arial, sans-serif; line-height: 1.6; color: #333; padding: 20px; }
        h1, h2 { color: #007bff; }
        p, ul { margin-bottom: 10px; }
        code { background: #f4f4f4; padding: 2px 4px; }
        .container { max-width: 800px; margin: auto; }
        .img-container { text-align: center; margin: 20px 0; }
        img { max-width: 100%; height: auto; }
    </style>
</head>
<body>
<div class="container">
    <h1>Final Project - Assembler</h1>
    <p>This project, developed by Adir Buskila and Niv Harosh for the System Programming Lab course at the Open University of Israel, is an assembler for a 16-instruction assembly language designed for an imaginary 14-bit CPU. The assembler translates assembly code into machine code, showcasing our hands-on experience with system software development.</p>

    <h2>Running the Program</h2>
    <p>To use the assembler, execute the following steps in a terminal within the project's directory:</p>
    <ol>
        <li>Compile the program with the <code>make</code> command.</li>
        <li>To assemble files, use <code>./assembler filename</code> without the <code>.as</code> extension.</li>
    </ol>
    <p>Example:</p>
    <pre><code>$ assembler program</code></pre>
    <p>If successful, the assembler generates four output files:</p>
    <ul>
        <li><code>.am</code> - The preprocessed source, with macros expanded and comments/empty lines removed.</li>
        <li><code>.ob</code> - The binary machine code.</li>
        <li><code>.ext</code> - Extern symbols used in the source.</li>
        <li><code>.ent</code> - Entry symbols defined in the source.</li>
    </ul>

    <div class="img-container">
        <img src="https://github.com/BAxPI/FinalProject/blob/main/images/am_output.png?raw=true" alt="program.am Output Example">
        <img src="https://github.com/BAxPI/FinalProject/blob/main/images/ob_output.png?raw=true" alt="program.ob Output Example">
    </div>

    <p>In case of errors, the assembler will report them to <code>stdout</code> and only produce the <code>.am</code> file.</p>
    <div class="img-container">
        <img src="https://github.com/BAxPI/FinalProject/blob/main/images/errors_output.png?raw=true" alt="Error Messages Example">
    </div>

    <h2>Directory Structure (Modules)</h2>
    <p>The project is organized into modules for clarity and maintainability:</p>
    <ul>
        <li><code>assembler</code> - Main function, argument processing.</li>
        <li><code>preprocessor</code> - Preprocesses the assembly code.</li>
        <li><code>first_pass</code> and <code>second_pass</code> - Implements the two-pass assembly process.</li>
        <li><code>globals.h</code> - Defines types and constants.</li>
        <li><code>lexer.h</code> and <code>lexer</code> - Constructs the Abstract Syntax Tree.</li>
        <li><code>code</code> and <code>directives</code> - Functions for encoding commands and directives.</li>
        <li><code>tables</code> - Manages Macros and Symbols tables.</li>
        <li><code>write_files</code> - Handles output file creation.</li>
        <li><code>utils</code> - General utility functions.</li>
    </ul>
    <p>Thank you for exploring our assembler project. We're open to contributions and suggestions to improve our assembler's functionality and efficiency. Feel free to fork the repository and submit pull requests!</p>
</div>
</body>
</html>
