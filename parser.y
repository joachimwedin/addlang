%{
    #include "node.hpp"
    Program *program; /* the top level root node of our final AST */

    extern int yylex();
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
%type <program> program
%type <expression> expression
%type <expression> numeral

%start program

%%

program:
                expression
                {
                        program = new Program(*$1);
                }
                ;

numeral:
                INTEGER
                {
                        $$ = new Integer(atol($1->c_str())); delete $1;
                }
                ;

expression:
                expression PLUS numeral
                {
                        $$ = new Add(*$1, *$3);
                }

                | numeral
                ;
