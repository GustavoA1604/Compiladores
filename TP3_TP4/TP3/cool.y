/*
 *  cool.y
 *              Parser definition for the COOL language.
 *
 */
%{
#include <iostream>
#include "cool-tree.h"
#include "stringtab.h"
#include "utilities.h"

#define YYLTYPE int
#define cool_yylloc curr_lineno

extern char *curr_filename;
extern int node_lineno;

#define YYLLOC_DEFAULT(Current, Rhs, N) \
  Current = node_lineno = Rhs[1];

void yyerror(char *s);        /*  defined below; called for each parse error */
extern int yylex();           /*  the entry point to the lexer  */

/************************************************************************/
/*                DONT CHANGE ANYTHING IN THIS SECTION                  */

Program ast_root;	      /* the result of the parse  */
Classes parse_results;        /* for use in semantic analysis */
int omerrs = 0;               /* number of errors in lexing and parsing */
%}

/* A union of all the types that can be the result of parsing actions. */
%union {
  Boolean boolean;
  Symbol symbol;
  Program program;
  Class_ class_;
  Classes classes;
  Feature feature;
  Features features;
  Formal formal;
  Formals formals;
  Case case_;
  Cases cases;
  Expression expression;
  Expressions expressions;
  char *error_msg;
}

/* 
   Declare the terminals; a few have types for associated lexemes.
   The token ERROR is never used in the parser; thus, it is a parse
   error when the lexer returns it.

   The integer following token declaration is the numeric constant used
   to represent that token internally.  Typically, Bison generates these
   on its own, but we give explicit numbers to prevent version parity
   problems (bison 1.25 and earlier start at 258, later versions -- at
   257)
*/
%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
%token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
%token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
%token <symbol>  STR_CONST 275 INT_CONST 276 
%token <boolean> BOOL_CONST 277
%token <symbol>  TYPEID 278 OBJECTID 279 
%token ASSIGN 280 NOT 281 LE 282 ERROR 283

/*  DON'T CHANGE ANYTHING ABOVE THIS LINE, OR YOUR PARSER WONT WORK       */
/**************************************************************************/
 
   /* Complete the nonterminal list below, giving a type for the semantic
      value of each non terminal. (See section 3.6 in the bison 
      documentation for details). */

/* Declare types for the grammar's non-terminals. */
%type <program> program
%type <classes> class_list
%type <class_> class
%type <features> feature_list
%type <features> feature_list_1
%type <feature> feature
%type <feature> attribute
%type <feature> method
%type <formals> formal_list
%type <formals> formal_list_1
%type <formal> formal
%type <cases> case_list
%type <case_> case
%type <expressions> expression_semicolon
%type <expressions> expression_comma
%type <expressions> expression_comma_1
%type <expression> expression
//%type <expressions> let_list
//%type <expression> let

/* Precedence declarations go here. */
/* According to the COOL manual
   .
   @
   ~
   isvoid
   * /
   + -
   <= < =
   not
   <- 
All binary operations are left-associative, with the exception of assignment, 
which is right-associative, and the three comparison operations, which do not 
associate */
%right ASSIGN
//%precedence LET
%nonassoc NOT
%nonassoc LE '<' '=' 
%left '-' '+'
%left '*' '/'
%nonassoc ISVOID
%nonassoc '~'
%nonassoc '@'
%nonassoc '.'

%%
/* 
   Save the root of the abstract syntax tree in a global variable.
*/
program	
  : class_list	{ @$ = @1; ast_root = program($1); }
  ;

class_list
	: class			/* single class */
		{ $$ = single_Classes($1);
                  parse_results = $$; }
	| class_list class	/* several classes */
		{ $$ = append_Classes($1,single_Classes($2)); 
                  parse_results = $$; }
  | class_list CLASS error feature_list '}' ';'
    { yyerrok; $$ = $1; }
  | class_list CLASS error feature_list ';'
    { yyerrok; $$ = $1; }
	;

/* If no parent is specified, the class inherits from the Object class. */
/* Class_ class_(Symbol name, Symbol parent, Features features, Symbol filename) */
class	
  : CLASS TYPEID '{' feature_list '}' ';'
		{ $$ = class_($2,idtable.add_string("Object"),$4,
			      stringtable.add_string(curr_filename)); }
	| CLASS TYPEID INHERITS TYPEID '{' feature_list '}' ';'
		{ $$ = class_($2,$4,$6,stringtable.add_string(curr_filename)); }
	;

/* Feature list may be empty, but no empty features in list. */
feature_list
  : /* empty */
    { $$ = nil_Features(); }
  | feature_list_1 /* 1 or more features */
    { $$ = $1; }
  ;

feature_list_1
  :	feature               /* single feature */
    { $$ = single_Features($1); }
  | feature_list_1 feature  /* several features */
    { $$ = append_Features($1, single_Features($2)); } 
  | feature_list_1 error ';'
    { yyerrok; $$ = $1; }
  ;

/* A feature is either an attribute or a method.*/
feature
  : attribute
  | method
  ;

/* Feature attr(Symbol name, Symbol type_decl, Expression init) */
attribute 
  : OBJECTID ':' TYPEID ';'
    { $$ = attr($1, $3, no_expr()); }
  | OBJECTID ':' TYPEID ASSIGN expression ';'
    { $$ = attr($1, $3, $5); }
  ;

/* Feature method(Symbol name, Formals formals, Symbol return_type, Expression expr) */
method
  : OBJECTID '(' formal_list ')' ':' TYPEID '{' expression '}' ';'
    { $$ = method($1, $3, $6, $8); }
  ;

formal_list
  : /* empty */
    { $$ = nil_Formals(); }
  | formal_list_1 /* 1 or more formals */
    { $$ = $1; }
  ;

formal_list_1
  : formal /* single formal */
    { $$ = single_Formals($1); }
  | formal_list_1 ',' formal /* several formals */
    { $$ = append_Formals($1, single_Formals($3)); }
  ;

/* Formal formal(Symbol name, Symbol type_decl) */
formal
  : OBJECTID ':' TYPEID
    { $$ = formal($1, $3); }
  ;

case_list
  : case /* single case */
    { $$ = single_Cases($1); }
  | case_list case /* several cases */
    { $$ = append_Cases($1, single_Cases($2)); }
  ;

case
  : OBJECTID ':' TYPEID DARROW expression ';'
    { $$ = branch($1, $3, $5); }
  ;

/*
let_list
  : let
    { $$ = single_Expressions($1); }
  | let_list ',' let
    { $$ = append_Expressions($1, single_Expressions($3)); }
  ;

let
  : OBJECTID ':' TYPEID
    {}
  | OBJECTID ':' TYPEID ASSIGN expression ';'
    {}
  ;
*/
expression_comma
  : /* empty */
    { $$ = nil_Expressions(); }
  | expression_comma_1 /* 1 or more expressions, comma separated */
    { $$ = $1; }
  ;

expression_comma_1
  : expression /* single expression */
    { $$ = single_Expressions($1); }
  | expression_comma ',' expression /* several expressions, comma separated */
    { $$ = append_Expressions($1, single_Expressions($3)); }
  ;

expression_semicolon
  : expression ';' /* single expression with ';' */
    { $$ = single_Expressions($1); } 
  | error ';'
    { yyerrok; $$ = nil_Expressions(); }
  | expression_semicolon expression ';' /* several expressions with ';' */
    { $$ = append_Expressions($1, single_Expressions($2)); }
  | expression_semicolon error ';'
    { yyerrok; $$ = $1; }
  ;

expression
  : OBJECTID ASSIGN expression { $$ = assign($1, $3); }
  | expression '.' OBJECTID '(' expression_comma ')'
    { $$ = dispatch($1, $3, $5); }
  | expression '@' TYPEID '.' OBJECTID '(' expression_comma ')'
    { $$ = static_dispatch($1, $3, $5, $7); }
  | OBJECTID '(' expression_comma ')'
    { $$ = dispatch(object(idtable.add_string("self")), $1, $3); }
  | IF expression THEN expression ELSE expression FI
    { $$ = cond($2, $4, $6); }
  | WHILE expression LOOP expression POOL
    { $$ = loop($2, $4); }
  | '{' expression_semicolon '}'
    { $$ = block($2); }
  | LET OBJECTID ':' TYPEID IN expression %prec LET
    { $$ = let($2, $4, no_expr(), $6); }
  | LET OBJECTID ':' TYPEID ASSIGN expression IN expression %prec LET
    { $$ = let($2, $4, $6, $8); }
  | LET error IN expression %prec LET
    { yyerrok; $$ = $4; }
  | CASE expression OF case_list ESAC
    { $$ = typcase($2, $4); }
  | NEW TYPEID { $$ = new_($2); }
  | ISVOID expression { $$ = isvoid($2); }
  | expression '+' expression { $$ = plus($1, $3); }
  | expression '-' expression { $$ = sub($1, $3); }
  | expression '*' expression { $$ = mul($1, $3); } 
  | expression '/' expression { $$ = divide($1, $3); }
  | '~' expression { $$ = neg($2); }
  | expression '<' expression { $$ = lt($1, $3); }
  | expression LE expression { $$ = leq($1, $3); }
  | expression '=' expression { $$ = eq($1, $3); }
  | NOT expression { $$ = comp($2); }
  | '(' expression ')' { $$ = $2; }
  | OBJECTID { $$ = object($1); }
  | INT_CONST { $$ = int_const($1); }
  | STR_CONST { $$ = string_const($1); }
  | BOOL_CONST { $$ = bool_const($1); }
  ;

/* end of grammar */
%%


/* This function is called automatically when Bison detects a parse error. */
void yyerror(char *s)
{
  extern int curr_lineno;

  cerr << "\"" << curr_filename << "\", line " << curr_lineno << ": " \
    << s << " at or near ";
  print_cool_token(yychar);
  cerr << endl;
  omerrs++;

  if(omerrs>50) {fprintf(stdout, "More than 50 errors\n"); exit(1);}
}

