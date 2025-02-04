/*  
    Name: Vamsi Krishna Talluri.
    Description: To implement a Lexical Analayzer which is used in compilers for tokenizing source codes which further used for error identification
    and processing.
    Date:04/02/2025
 */


#include"lex.h"
FILE *fptr;
int main(int argc, char* argv[]) // to recieve command line arguments (i.e '.c' file)
{
    if(cla_validate(argc, argv) == failure) // to validate the command line arguments
    {
        return failure;
    }
    
    fptr = fopen(argv[1],"r");
    if(fptr == NULL)
    {
        return failure;
    }

    print_analysis();
    return success;
}