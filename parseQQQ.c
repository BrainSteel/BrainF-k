#include "stdlib.h"
#include "stdio.h"

#include "common.h"
#include "parse.h"

bf_command* ParseQQQ(const char* file, int* num, int* read, int* error){
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
    int end = 0, halt = 0, open = 1;
    int current = -1;
    char c = getc(fp);
    char last = 'A';
    
    while (!end && !halt) {
        switch (c) {
            case EOF:
                end = 1;
                break;
                
            case '.':
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
                        prgm[current].type = bf_add;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case ',':
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
                        prgm[current].type = bf_subtract;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case ';':
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
                        prgm[current].type = bf_right;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case '-':
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
                        prgm[current].type = bf_left;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case '?':
                prse_read++;
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
                    prgm[current].type = bf_input;
                    prgm[current].val = -1;
                }
                break;
                
            case '!':
                prse_read++;
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
                break;
                
            case '\"':
                prse_read++;
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
                    if (open)
                        prgm[current].type = bf_open;
                    else
                        prgm[current].type = bf_close;
                    prgm[current].val = -1;
                }
                break;
                
            case '\'':
                prse_read++;
                open = !open;
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
                    current = prse_num - 1;
                    prgm[current].type = bf_debug;
                    prgm[current].val = -1;
                }
                break;
            default:
                break;
        }
        
        last = c;
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


void PrintQQQ(const char* file, bf_command* prgm){
    FILE* fp = stdout;
    if (file) {
        fp = fopen(file, "w");
        if (!fp) {
            fp = stdout;
        }
    }
    
    int i = 0, count, open = 1;
    while (prgm[i].type != bf_end) {
        switch (prgm[i].type) {
            case bf_add:
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, ".");
                }
                break;
            case bf_subtract:
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, ",");
                }
                break;
            case bf_right:
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, ";");
                }
                break;
            case bf_left:
                for (count = 0; count < prgm[i].val; count++) {
                    fprintf(fp, "-");
                }
                break;
            case bf_input:
                fprintf(fp, "?");
                break;
            case bf_output:
                fprintf(fp, "!");
                break;
            case bf_open:
                if (!open) {
                    fprintf(fp, "\'");
                    open = !open;
                }
                fprintf(fp, "\"");
                break;
                
            case bf_close:
                if (open) {
                    fprintf(fp, "\'");
                    open = !open;
                }
                fprintf(fp, "\"");
                break;
                
            default:
                break;
        }
        i++;
    }
}