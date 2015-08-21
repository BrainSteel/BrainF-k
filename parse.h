#ifndef BF_PARSE_H_INCLUDED
#define BF_PARSE_H_INCLUDED

/* Parse commands for BrainF>>k Interpreter */

#include "common.h"

#define PARSE_SUCCESS   0
#define PARSE_MEMORY    1
#define PARSE_BRACKETS  2

typedef bf_command* (*ParseFunction) (const char* file, int* num, int* read, int* error);
typedef void (*PrintFunction) (const char* file, bf_command* prgm);

/* Connect brackets parsed (optionally used by parse functions) 
   Parse.c
*/
extern int matchBrackets(bf_command* prgm);

/* Master language list X-Macro (id, name) */
#define X_LANGUAGES  X(BF, "BF")    \
X(QQQ, "???")                       \
X(TBF, "TinyBF")

/* Declare print and parse functions for the language list */
#define X(id, name)                                                                     \
extern bf_command* Parse##id(const char* file, int* num, int* read, int* error);        \
extern void Print##id(const char* file, bf_command* prgrm);

X_LANGUAGES

#undef X

#endif
