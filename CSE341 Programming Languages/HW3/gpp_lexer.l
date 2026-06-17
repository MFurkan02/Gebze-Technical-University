%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
%}

%%

"+"     { return OP_PLUS; }
"-"     { return OP_MINUS; }
"/"     { return OP_DIV; }
"*"     { return OP_MULT; }
"("     { return OP_OP; }
")"     { return OP_CP; }
","     { return OP_COMMA; }

"and"   { return KW_AND; }
"or"    { return KW_OR; }
"not"   { return KW_NOT; }
"equal" { return KW_EQUAL; }
"less"  { return KW_LESS; }
"nil"   { return KW_NIL; }
"list"  { return KW_LIST; }
"append" { return KW_APPEND; }
"concat" { return KW_CONCAT; }
"set"   { return KW_SET; }
"def"   { return KW_DEF; }
"for"   { return KW_FOR; }
"if"    { return KW_IF; }
"exit"  { return KW_EXIT; }
"load"  { return KW_LOAD; }
"display" { return KW_DISPLAY; }
"true"  { return KW_TRUE; }
"false" { return KW_FALSE; }

";;".*  { /* Ignore comments */ }

[0-9]+f[0-9]+ {
    
    strcpy(yylval.val, yytext);
    return VALUEF;
}

[a-zA-Z][a-zA-Z0-9]* {
    strcpy(yylval.id, yytext);
    return IDENTIFIER;
}

[\t ] { } // Ignore white spaces

. {
}

%%

int yywrap() {
    return 1;
}
