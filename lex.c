#include "lex.h"

//length is 16;
char operators[] = {'+','-','*','/','%','=','!','<','>','|','&','^','~','?','.',','};

//length  is 4
char specialCharacters[] = {':', ';' , '[' , ']' };

//2operators
//length 17
//char multi_operators[][3] = { {'&','&'}, {'|','|'}, {'<','='}, {'>','='}, {'=','='}, {'!','='}, {'+','+'}, {'-','-'}, {'-','>'}, {'<','<'}, {'>','>'}, {'&','='}, {'|','='}, {'^','='}, {'*','='}, {'/','='}, {'%','='} };
char multi_operators[][3] = { "&&", "||", "!=", "==", "<=", ">=", "*=", "/=", "%=", "++", "--", "<<", ">>", "&=", "|=", "^=", "->" };

//32 keywords.
char *keywords[] = { "int", "char", "float", "double", "short", "long", "auto", "static", "register", "extern", "for", "do", "while", "if", "else", 
                    "goto",	"signed", "unsigned", "struct", "enum", "switch", "case", "break", "continue", "default", "typedef", "union", "void", "sizeof", "const", "volatile","return"};


char array[100];
int array_index = 0;
char ch;

int line_count = 0;

extern FILE *fptr;

status cla_validate(int argc,char *argv[])
{
    if(argc < 2 || argc > 2)
    {
        printf("Usage: ./a.out < filename.c >\n");
        return failure;
    }
    
    if(strstr(argv[1],".c") == NULL)
    {
        printf("\033[1;31mERROR: Invalid file format\033[0m\n");
        return failure;
    }
    return success;
}

int skip_preprocessordirectives()
{
    printf("skipped header\n");
    fscanf(fptr,"%*[^\n]");
}

status skip_comments()
{
    char ch = getc(fptr); 
    if(ch != EOF && ch == '*')
    {
        while((ch = getc(fptr)) != EOF && ch != '/')
        {
            continue;
        }
        getc(fptr); //for reading '/'
        printf("skipped multi-line comment\n");
    }
    else if(ch != EOF && ch == '/' )
    {
        printf("skipped single line comment\n");
        fscanf(fptr,"%*[^\n]");
    }
    else
    {
        //if both the conditions are failed then it is not comment part so seek back one position from current position.
        fseek(fptr,-1,SEEK_CUR);
        return failure;
    }
}

status string_literal_check(char ch)
{
    int double_quotes = 0;
    if(ch == '"')
    {
        double_quotes++;
        //insert into array
        array[array_index++] = ch;
        //read next following characters upto closing aphestrophe ( " )
        while((ch = getc(fptr)) != EOF && (ch != '"') && (ch != '\n'))
        {
            array[array_index++] = ch;
        }
        
        //insert '\0'
        
        //insert that failed character i.e ( " )
        if(ch == '"')
        {
            array[array_index++] = ch;
            array[array_index++] = '\0';
            //if the loop is terminates because of " then decrement the double quotes.
            double_quotes--;
            printf("String Literal : %s\n",array);

        }
        if(double_quotes != 0)
        {
            array[array_index++] = '\0';
            printf("\033[1;31mERROR : Missing \" in String literal\033[0m\n");
        }
        //array
        //reset array index.
        array_index = 0;
        return success;  
    }
    return failure;
}

status number_check(char ch)
{
    int flag = 0;
    //if num then insert into array
    if(ch >= '0' && ch <= '9')
    {
        //before entering the while loop for reading folowing characters
        //insert the current character.
        if(ch == '0')
        {
            array[array_index++] = ch;
            //check whether it is hexa, octal, or binary
            ch = getc(fptr);
            if(ch == 'x' || ch == 'X')
            {
                //insert into array
                array[array_index++] = ch;
                
                while((ch = getc(fptr)) != EOF && (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '.') && (ch != ';') && (ch != '\n'))
                {
                    array[array_index++] = ch;
                }
                fseek(fptr,-1,SEEK_CUR);
                //insert '\0'
                array[array_index++] = '\0';
                
                //run a loop to check whether constant having out of range notation.
                int i=2; 
                while(array[i] != '\0')
                {
                    if((array[i] >= 'G' && array[i] <= 'Z') || (array[i] >= 'g' && array[i] <= 'z') || (ch == '.') )
                    {
                        printf("\033[1;31mERROR: Hexa constant \"%s\" with illigal notation\033[0m\n",array);
                        flag = 1;
                        break;
                    }
                    i++;
                }
                if(flag == 0)
                {
                    printf("Hexa Constant : %s\n",array);
                }
                //reset flag = 0;
                flag = 0;
                //reset array index.
                array_index = 0;
                return success;
            }
            else if(ch == 'b' || ch == 'B')
            {
                //insert into array
                array[array_index++] = ch;
                
                //while((ch = getc(fptr)) != EOF && ch >= '0' && ch <= '1')
                while((ch = getc(fptr)) != EOF && (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '.') && (ch != ';') && (ch != '\n'))
                {
                    array[array_index++] = ch;
                }
                //seek back for false condition
                fseek(fptr,-1,SEEK_CUR);
                
                //insert '\0'
                array[array_index++] = '\0';

                //check why previous condition failed
                //check whether character is out of hexa range or not.
                int i=2;
                while(array[i] != '\0')
                {
                    if(ch !=  '0' || ch != '1')
                    {
                        printf("\033[1;31mERROR: Binary constant \"%s\" with illigal notation\033[0m\n",array);
                        flag = 1;
                        break; 
                    }
                    i++;
                }

                if(flag == 0)
                {
                    printf("Binary Constant : %s\n",array); 
                }
                //reset flag =0;
                flag = 0; 
                //reset array index.
                array_index = 0;
                return success;
            }
            else if( ch >= '0' && ch <= '7') 
            {
                array[array_index++] = ch;
                while((ch = getc(fptr)) != EOF && (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '.') && (ch != ';') && (ch != '\n'))
                //while((ch = getc(fptr)) != EOF && ch >= '0' && ch <= '7')
                {
                    array[array_index++] = ch;
                }
                //seek back for false condition
                fseek(fptr,-1,SEEK_CUR);

                //insert '\0'
                array[array_index++] = '\0';

                //check why previous condition failed
                //check whether character is out of hexa range or not.
                int i=1;
                while(array[i] != '\0')
                {    
                    if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '9') || (ch == '.') || (ch == '8') )
                    {
                        printf("\033[1;31mERROR: Octal constant \"%s\" with illigal notation\033[0m\n",array);
                        flag = 1;
                        break;
                    }
                    i++;
                }
                if(flag == 0)
                {
                    printf("Octal Constant : %s\n",array);
                }
                //reset flag = 0
                flag = 0;
                //reset array index.
                array_index = 0;
                return success;
            }
            else
            {
                //for single digit character directly print the character
                printf("Integral Constant : %c\n",array[0]);
                //seek back 1-position beacause we read one more character for validation. 
                fseek(fptr,-1,SEEK_CUR);
                //reset the array index.
                array_index = 0; 
                return success;
            }
        }
        else
        {
            //insert that charcater into array.
            array[array_index++] = ch;
            while((ch = getc(fptr)) != EOF && (ch >= '0' && ch <= '9') || (ch == 'F') || (ch == 'f') || ch == '.')
            {
                array[array_index++] = ch;
            }
            //seek back for false condition
            fseek(fptr,-1,SEEK_CUR);

            array[array_index++] = '\0';
            //compare it is floating constant or integer constant
            if(strchr(array,'.') != NULL)
            {
                printf("Floating Constant : %s\n",array);
            }
            else
            {
                printf("Integral Constant : %s\n",array);
            }
            
            //reset array index.
            array_index = 0;
            return success;
        }
    }
    else
    {
        return failure;
    }
}

status operator_check(char ch)
{
    for(int i=0; i<16; i++)
    {
        if(ch == operators[i])
        {
            array[array_index++] = ch;
            //read second character to check for 2 character operator.
            if((ch = getc(fptr)) != EOF && (ch != ' ') && (ch == '+' || ch == '-' || ch == '=' ||  ch == '&' || ch == '|' || ch == '>' || ch == '<'))
            {
                array[array_index++] = ch;
                array[array_index++] = '\0';
                //if true now insert that char also into array.
                printf("2-Character Operator : %s\n",array);
                //reset the array index;
                array_index = 0;
                return success;
            }
            else
            {
                //else if not a operator
                //seek back to previous char;
                fseek(fptr,-1,SEEK_CUR);
                //and print the previous character which is already stored in array of index - 0
                printf("Operator : %c\n",array[0]);
                //reset the array index to 0 
                array_index = 0;
                return success;
            }
        }
       
    }
    return failure;
}

status special_character_check(char ch)
{
    for(int i=0; i<8; i++)
    {
        if(ch == specialCharacters[i])
        {
            printf("Special character : %c\n",ch);
            return success;
        }
    }
    return failure;
}

status character_check(char ch)
{
    if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '_') )
    {
        //if true 
        //insert into array and loop for reading next following characters
        array[array_index++] = ch;
        while((ch = getc(fptr)) != EOF &&  ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '_') || (ch >= '0' && ch <= '9') && ch != ' '))
        {
            array[array_index++] = ch;
        }
        //seek back for false condition.
        fseek(fptr,-1,SEEK_CUR);
        //insert '\0'
        array[array_index++] = '\0'; 
        
        int flag = 0;

        //now compare with all the keywords in keywords array.
        for(int i=0; i<32; i++)
        {
            if(strcmp(array,keywords[i]) == success)
            {
                // printf("printing keyword\n");
                printf("Keyword : %s\n",array);
                array_index = 0;
                return success;
            }
        }

        // if(flag == 0)
        {
            // printf("printing Identifier\n");
            printf("Identifier : %s\n",array);
            array_index = 0;
            return success;
        }
    }
}

void print_analysis()
{
    int parenthesis = 0;
    int curl_bracket = 0;
    while((ch = getc(fptr)) != EOF)
    {
    //   printf("hi\n");
        if(ch == '#')
        {
            skip_preprocessordirectives();
        }
        else if(ch == ' ' || ch == '\n')
        {
            continue;
        }
        else if(ch == '/')
        {
            if(skip_comments() == success)
            {
                continue;
            } 
            else
            {
                printf("Operator : /\n");
            }
        }
        else if(ch == '\'')
        {
            //update it to array
            int single_quote = 1;
            array[array_index++] = ch;
            int count_of_ch_in_single_quote=0;
            while( ((ch = getc(fptr)) != EOF)  && (ch != '\'') && (ch != '\n') && (ch != ';'))
            {
                count_of_ch_in_single_quote++;
                array[array_index++] = ch;
            }
            fseek(fptr,-1,SEEK_CUR);
            if(ch == '\'')
            {
                //update closing single quote also
                array[array_index++] = ch;
                single_quote--;
            }
            
            if(single_quote != 0)
            {
                printf("\033[1;31mERROR: Missing \' apostrophe\033[0m\n");
            }
            else
            {
                //insert '\0'
                array[array_index++] = '\0';
                if(count_of_ch_in_single_quote == 1)
                {
                    printf("String constant : %s\n",array);
                } 
                else
                {
                    printf("\033[1;31mERROR: %s\033[0m\n",array);
                }
            }
            // reset the array index
            array_index = 0;
        }
        else if(ch == '(')
        {
            printf("Special character : (\n");
            int position = 0;
            int flag = 0; 
            while( (ch = getc(fptr)) != EOF && (ch != '\n'))
            {
                position++;
                if(ch == ')')
                {
                    parenthesis = 1;
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                printf("\033[1;31mERROR : Missing )\033[0m\n");
            }

            fseek(fptr,-position,SEEK_CUR);
        }
        else if(ch == ')')
        {
            if(parenthesis == 1)
            {
                parenthesis = 0;
                printf("Special character : )\n");
            }
            else
            {
                printf("\033[1;31mERROR: ) without (\033[0m\n");
            }
        }
        else if(ch == '{')
        {
            printf("Special Character : {\n");
            curl_bracket++;
        }
        else if(ch == '}')
        {
            printf("Special Character : }\n");
            curl_bracket--;
        }
        else if(string_literal_check(ch) == success)
        {
            //printf("operator printed\n");
        }
        //check if it is an operator or not.
        else if(operator_check(ch) == success)
        {
            //printf("operator printed\n");
        }
        else if(special_character_check(ch) == success)
        {
            //printf("operator printed\n");
        }
        else if(number_check(ch) == success)
        {
            //printf("number printed\n");
        }
        else if(character_check(ch) == success)
        {

        }
        else
        {
            continue;
        }
    }
        if(curl_bracket != 0)
        {
            printf("\033[1;31mERROR : Missing '{' '}'\033[0m\n");
        }
    fclose(fptr);
}
