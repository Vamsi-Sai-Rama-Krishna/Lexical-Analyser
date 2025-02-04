# Lexical-Analyser

## 📌 Project Overview
This **Lexical Analyzer** project resembels a fundamental part of a compiler,that is responsible for **tokenizing** input source code such as **keywords, identifiers, operators, and literals**. It serves as the first phase in the compilation process.
The main purpose of this project is to convert source code of ".c" file which is passed through command line argument into specific tokens which are further passed to next stage of compilation.

## Steps and Sample Result
1. **Compilation**
   ```bash
   gcc main.c lex.c lex.h
   ```
2. **Execution**
   ```bash
   ./a.out <sample_c_code.c>
   ```
3. **Sample C code**
   ```bash
   #include<stdio.h>
   //sample code for lexical analyzer project.
   int main()
   {
     int num = 10;
     char *str = "Hello World";    
   }
   ```
4. **Output Example**
   ```
   skip preprocessor directives
   skip single line comments
   keyword: int 
   Identifier: main
   Special Character: (
   Special Character: )
   Special Character: {
   keyword: int 
   Identifier: num
   Operator: =
   Integral Constant: 10
   Special Character: ;
   keyword: char 
   Operator: *
   Identifier: str
   Operator: =
   String Literal: "Hello World"
   Special Character: ;
   ```

