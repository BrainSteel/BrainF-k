#include "stdlib.h"
#include "stdio.h"

#include "common.h"
#include "parse.h"
#include "stdlib.h"
#include "stdio.h"

#include "common.h"
#include "parse.h"


bf_command* ParseTBF(const char* file, int* num, int* read, int* error){
    /* Our own counters */
    int prse_error = 0, prse_read = 0, prse_num = 0;
    
    /* Loading the file */
    FILE* fp = fopen(file, "r");
    if (!fp) {
        printf("Parse error: could not read file.\n");
        return NULL;
    }
    
    /* Initializations */
    bf_command* prgm = NULL;
    bf_command* tmp;
    int end = 0, halt = 0, direction = 1;
    int current = -1;
    char c = getc(fp);
    char last = 0, lastlast = 0;
    
    while (!end && !halt) {
        switch (c) {
            case EOF:
                end = 1;
                break;
                
            case '+':
                prse_read++;
                if (last == c) {
                    prgm[current].val++;
                }
                else {
                    tmp = realloc(prgm, (prse_num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", prse_num);
                        prse_error = PARSE_MEMORY;
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        prse_num++;
                        current = prse_num - 1;
                        if (direction) {
                            prgm[current].type = bf_add;
                        }
                        else prgm[current].type = bf_subtract;
                        prgm[current].val = 1;
                    }
                }
                lastlast = last;
                last = c;
                break;
                
            case '>':
                prse_read++;
                if (last == c) {
                    prgm[current].val++;
                }
                else {
                    tmp = realloc(prgm, (prse_num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", prse_num);
                        prse_error = PARSE_MEMORY;
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        prse_num++;
                        current = prse_num - 1;
                        if (direction) {
                            prgm[current].type = bf_right;
                        }
                        else prgm[current].type = bf_left;
                        prgm[current].val = 1;
                    }
                }
                lastlast = last;
                last = c;
                break;
                
            case '|':
                prse_read++;
                if (last == '=' && lastlast == '|') {
                    /* '|=|' means 'take input', so we overwrite the last written command (the '|') */
                    current = prse_num - 1;
                    prgm[current].type = bf_input;
                    prgm[current].val = -1;
                    /* `|=|=|` should be input, then switch bracket. This replaces it with |=A=|
                        so that a second input isn't triggered                                      */
                    c = 0;
                    
                    /* We also need to un-toggle direction */
                    direction = !direction;
                }
                else {
                    tmp = realloc(prgm, (prse_num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", prse_num);
                        prse_error = PARSE_MEMORY;
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        prse_num++;
                        current = prse_num - 1;
                        if (direction) {
                            prgm[current].type = bf_open;
                        }
                        else prgm[current].type = bf_close;
                        prgm[current].val = -1;
                    }
                }
                lastlast = last;
                last = c;
                break;
                
            case '=':
                prse_read++;
                direction = !direction;
                if (last == '=') {
                    /* '==' means output */
                    tmp = realloc(prgm, (prse_num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", prse_num);
                        prse_error = PARSE_MEMORY;
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        prse_num++;
                        current = prse_num - 1;
                        prgm[current].type = bf_output;
                        prgm[current].val = -1;
                    }
                    c = 0;
                }
                lastlast = last;
                last = c;
                break;
                
            case '~':
                tmp = realloc(prgm, (prse_num + 1) * sizeof(bf_command));
                if (!tmp) {
                    printf("Parser error! Ran out of memory at command %d.\n", prse_num);
                    prse_error = PARSE_MEMORY;
                    prgm[current].type = bf_end;
                    halt = 1;
                }
                else {
                    prgm = tmp;
                    prse_num++;
                    current = prse_num - 1;
                    prgm[current].type = bf_debug;
                    prgm[current].val = -1;
                }
                lastlast = last;
                last = c;
                break;
                
            default:
                break;
        }
        c = getc(fp);
    }
    
    if (!halt) {
        tmp = realloc(prgm, (prse_num + 1) * sizeof(bf_command));
        if (!tmp) {
            printf("Parser error! Ran out of memory at command %d.\n", prse_num);
            prse_error = PARSE_MEMORY;
            prgm[current].type = bf_end;
            halt = 1;
        }
        else {
            prgm = tmp;
            prse_num++;
            current = prse_num - 1;
            prgm[current].type = bf_end;
            prgm[current].val = 1;
        }
    }
    
    prse_error |= matchBrackets(prgm);
    
    if (read) {
        *read = prse_read;
    }
    if (num) {
        *num = prse_num;
    }
    if (error) {
        *error = prse_error;
    }
    
    return prgm;
}


void PrintTBF(const char* file, bf_command* prgm){
    FILE* fp = stdout;
    if (file) {
        fp = fopen(file, "w");
        if (!fp) {
            fp = stdout;
        }
    }
    int direction = 1;
    
    int i = 0, count;
    while (prgm[i].type != bf_end) {
        switch (prgm[i].type) {
            case bf_add:
                if (!direction) {
                    printf("=");
                    direction = !direction;
                }
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, "+");
                }
                break;
            case bf_subtract:
                if (direction) {
                    printf("=");
                    direction = !direction;
                }
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, "+");
                }
                break;
            case bf_right:
                if (!direction) {
                    printf("=");
                    direction = !direction;
                }
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, ">");
                }
                break;
            case bf_left:
                if (direction) {
                    printf("=");
                    direction = !direction;
                }
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, ">");
                }
                break;
            case bf_input:
                fprintf(fp, "|=|");
                break;
            case bf_output:
                fprintf(fp, "==");
                break;
            case bf_open:
                if (!direction) {
                    printf("=");
                    direction = !direction;
                }
                fprintf(fp, "|");
                break;
                
            case bf_close:
                if (direction) {
                    printf("=");
                    direction = !direction;
                }
                fprintf(fp, "|");
                break;
                
            default:
                break;
        }
        i++;
    }
}