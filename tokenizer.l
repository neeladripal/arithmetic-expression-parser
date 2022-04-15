%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?       {yylval.num = atof(yytext); return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]*  {yylval.id = yytext; return ID;}
[-+\*/\(\)=;\n]         {return yytext[0];}
[ \t]                   ;
\\                      {printf("end\n"); return 0;}
.                       {printf("unexcepted character\n");}
%%

int yywrap() {
    return 1;
}