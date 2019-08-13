%{
    #include "node.hpp"
    Program *programRoot; /* the top level root node of our final AST */
    #define YYDEBUG 1
    extern int yylex();
    extern FILE *yyin;
    void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

/* Represents the many different ways we can access our data */
%union {
    Program* program;
    Expression* expression;
    Integer* integer;
    std::string* string;
    int token;
}
%code provides {
const char* token_string(int token);
}

%define parse.trace

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> INTEGER
%token <token> PLUS

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <program> prog
%type <expression> expression
%type <expression> numeral

%start prog

%%

prog:
                expression
                {
                        programRoot = new Program($1);
                }
                ;

numeral:
                INTEGER
                {
                        $$ = new Integer(stoi(*$1)); delete $1;
                }
                ;

expression:
                expression PLUS numeral
                {
                        $$ = new Add($1, $3);
                }

                | numeral
                ;

%%
const char* token_string(int token) {
        return yytname[yytranslate[token]];
}
