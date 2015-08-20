#include "stdio.h"

#include "common.h"
#include "parse.h"

int matchBrackets(bf_command* prgm) {
    int i = 0, j = 0;
    int count = 0;
    while (prgm[i].type != bf_end) {
        if (prgm[i].type == bf_open) {
            j = i + 1;
            while (prgm[j].type != bf_end) {
                if (prgm[j].type == bf_open) {
                    count++;
                }
                else if (prgm[j].type == bf_close) {
                    count--;
                }
                if (count == -1) {
                    prgm[i].val = j;
                    prgm[j].val = i;
                    count = 0;
                    break;
                }
                j++;
            }
            
            if (prgm[j].type == bf_end) {
                printf("Error: Mismatched brackets.\n");
                return PARSE_BRACKETS;
            }
        }
        if (prgm[i].type == bf_close && prgm[i].val == -1) {
            printf("Error: Mismatched brackets.\n");
            return PARSE_BRACKETS;
        }
        i++;
    }
    return 0;
}