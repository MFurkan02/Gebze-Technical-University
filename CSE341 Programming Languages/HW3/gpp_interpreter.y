%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
int yylex();
int yyerror(const char * ch);
void simplify(int *numerator, int *denominator);
char* operation(char operator, char *val1, char *val2);
int ext=0;
int flag=0;
int err=0;
char op;
%}

%start START
%token OP_OP OP_CP OP_PLUS OP_MINUS OP_MULT OP_DIV 
%token KW_EXIT KW_DEF
%token OP_COMMA KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL KW_APPEND KW_CONCAT KW_SET KW_FOR KW_IF KW_LOAD KW_DISPLAY KW_TRUE KW_FALSE KW_LIST


%union {
    char id[50];
    char val[20];
    char expr[20];
}

%token <val> VALUEF
%token <id> IDENTIFIER
%type <expr> EXP



%%

START: EXP | FUNCTION | OP_OP KW_EXIT OP_CP { ext=1 ; return 0; }

EXP: OP_OP OP_PLUS EXP EXP OP_CP { 
    
     if(flag==1){ // if valuef is entered from the user do the operation.
       strcpy($$,operation('+', $3, $4)); printf("%s",$$);
     } 
     else if(flag==0){ // if identifier is entered from the user for the function use state the operation will be used for the function.
        op='+';
     }
     flag=0 ;
    }
    | OP_OP OP_MINUS EXP EXP OP_CP { 
             if(flag==1){ // if valuef is entered from the user do the operation.
        strcpy($$,operation('-', $3, $4));  printf("%s",$$);
        }
        else if(flag==0){ // if identifier is entered from the user for the function use state the operation will be used for the function.
            op='-';
        }
        flag=0;
    }
    | OP_OP OP_MULT EXP EXP OP_CP { 
        if(flag==1){ // if valuef is entered from the user do the operation.
        strcpy($$,operation('*', $3, $4)); printf("%s",$$);
        }
        else if(flag==0){ // if identifier is entered from the user for the function use state the operation will be used for the function.
            op='*';
        }
        flag=0;
    }
    | OP_OP OP_DIV EXP EXP OP_CP { 
        if(flag==1){ // if valuef is entered from the user do the operation.
            strcpy($$,operation('/', $3, $4)); printf("%s",$$); } 
        else if(flag==0){ // if identifier is entered from the user for the function use state the operation will be used for the function.
            op='/';
        } 
        flag=0;
    }
    | OP_OP IDENTIFIER EXP OP_CP { strcpy($$,operation(op, $3,"1f1")); printf("%s",$$); }
    | OP_OP IDENTIFIER EXP EXP OP_CP { strcpy($$,operation(op, $3, $4)); printf("%s",$$);} 
    | OP_OP IDENTIFIER EXP EXP EXP OP_CP { strcpy($$,operation(op, $3, $4)); printf("%s",$$);}
    | IDENTIFIER {  flag=0; strcpy($$,$1) ; } 
    | VALUEF { flag=1 ; strcpy($$,$1) ;  }
    ;

FUNCTION: OP_OP KW_DEF IDENTIFIER EXP OP_CP { flag=2 ; printf("#function"); }
    | OP_OP KW_DEF IDENTIFIER IDENTIFIER EXP OP_CP { flag=2; printf("#function"); }
    | OP_OP KW_DEF IDENTIFIER IDENTIFIER IDENTIFIER EXP OP_CP { flag=2 ;printf("#function"); }
    ;

%%



void simplify(int *numerator, int *denominator) {
    if(*numerator > *denominator) {
      for(int i=*denominator; i > 0; --i){
         if((*numerator % i == 0) && (*denominator % i == 0)){
             *numerator /= i ;
             *denominator /= i ;
             break ;
         }
      }
    } else if(*numerator < *denominator) {
        for(int i=*numerator; i > 0; --i){
            if((*denominator % i == 0) && (*numerator % i == 0)){
                *numerator /= i ;
                *denominator /= i ;
                break ;
            }
        }
    }
}

char* operation(char operator, char val1[], char val2[]) {
    int num1, den1, num2, den2, res_num, res_den;
    // Extract values using sscanf
    sscanf(val1, "%df%d", &num1, &den1);
    sscanf(val2, "%df%d", &num2, &den2);

    if(den1==0 || den2==0){
        printf("Division by zero! Syntax Error!\n");
        return 0;
    }

    switch (operator) {
        case '+':
            if(den1==den2){
            res_num = num1 + num2;
            res_den = den1;
            }
            else{
                res_num = num1*den2 + num2*den1;
                res_den = den1*den2;
            }
            break;
        case '-':
            if(den1==den2){
            res_num = num1 - num2;
            res_den = den1;
            }
            else{
                res_num = num1*den2 - num2*den1;
                res_den = den1*den2;
            }
            break;
        case '*':
            res_num = num1 * num2;
            res_den = den1 * den2;
            break;
        case '/':
            res_num = num1 * den2;
            res_den = den1 * num2;
            break;

        default:
            res_num = 0;
            res_den = 1;
    }
    simplify(&res_num,&res_den);
    
    
    char *result;
    result = (char *)malloc(sizeof(char)*10) ;
    sprintf(result, "%df%d\n", res_num, res_den);
    return result;

}

int yyerror(const char * ch) {
    err=1;
}

int main(){ 
    FILE *fp; 
    char terminal_input[70];
    char inputFile[80];
    char line[100];
    printf ("Enter g++ for terminal input.\n");
    printf ("Enter g++ 'filename' for file input.\n");
    printf("$ "); 

    
    fgets(terminal_input,sizeof terminal_input,stdin); // input from the user
    if (strncmp(terminal_input,"g++",3) !=0 ){      // if the first 3 letters are not g++
        printf("Invalid input! Program terminated!\n");
        
    }      
    else{

        if(strlen(terminal_input) == 4){
            while (ext!=1) {
                printf("> ");
                fgets(line, sizeof line, stdin);
                yyin = fmemopen(line, strlen(line), "r");
                yyparse();
                
                if(err==1){
                    printf("Syntax Error!\n");
                    yyparse();
                    err=0;
                }
               fclose(yyin);
               flag=0;
            }
            
        }
        else{
            strncpy(inputFile, terminal_input+4,strlen(terminal_input)-4); // to get the file name

            inputFile[strlen(terminal_input)-5] = '\0';
            fp = fopen(inputFile,"r"); 
            if(fp==NULL){
                printf("File does not exist!");
                return 0;
            }
            printf("-------------\n");
            while(fgets(line, sizeof(line),fp) ){
                yyin = fmemopen(line, strlen(line), "r");
                if (yyin == NULL) {
                    perror("Error opening memory stream");
                    exit(EXIT_FAILURE);
                }
                if(ext==1) return 0; 
                yyparse();
                
                if(err==1){
                    printf("Syntax Error!\n");
                    yyparse();
                    err=0;
                }
                fclose(yyin);
            }
        }
            
    }
         
    
 return 0;
}