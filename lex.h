#ifndef LEX_H
#define LEX_H
#include <stdio.h>
#include <string.h> 

// enum for status.
typedef enum
{
    success,
    failure
}status;


/* Fuction prototypes */

status cla_validate(int argc,char *argv[]);

int skip_preprocessordirectives();

/* Functions for printing analysis */
void print_analysis();

status skip_comments();

status operator_check(char ch);

status number_check(char ch);

status character_check(char ch);

status special_character_check(char ch);

status string_literal_check(char ch);

/* Functions for error finding */
void print_errors();

status check_double_quotes();

status parenthesis_check();

#endif
