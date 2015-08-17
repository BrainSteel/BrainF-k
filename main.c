/* 
 Brainf>>k
 BF interpreter written by @BrainSteel at PPCG.SE
 http://codegolf.stackexchange.com/users/31054/brainsteel
 
 Usage:
 
 bf file.bf [-d]
 
 8/11/15 V1.2.0
 '~' Prints the tape up to the position of the current pointer if the -d option is enabled
 No longer freaks out about mismatched brackets in some circumstances.
 
 11/20/14 V1.1.0
 Command line arguments accepted
 
 11/17/14 V1.0.0
 Full interpreter
 Minor optimizations
 Flag -d for diagnostics
 
 */



#include "stdio.h"
#include "stdlib.h"

#define VERSION_MAJOR 1
#define VERSION_PATCH 2
#define VERSION_TEST 0

typedef enum {
    bf_end,
    bf_add,
    bf_subtract,
    bf_right,
    bf_left,
    bf_output,
    bf_input,
    bf_open,
    bf_close,
    bf_print
} bf_type;

typedef struct {
    bf_type type;
    int val; /* Will represent the magnitude of increments, or the pointer to jump to... */
} bf_command;

bf_command* load(FILE* file);
void run(bf_command* prgm);
void printprogram(bf_command* prgm);

unsigned char RAM[30000];
int halt = 0;
int print_diagnostics = 0;
int bf_run = 0;
int cmd_run = 0;

int main(int argc, const char * argv[]) {
    if (argc < 2)
        printf("Brainf>>k Interpreter V%d.%d.%d\n", VERSION_MAJOR, VERSION_PATCH, VERSION_TEST);
    char cfile[500];
    
ask_for_file:
    bf_run = 0;
    cmd_run = 0;
    print_diagnostics = 0;
    
    if (argc >= 2) {
        int iii;
        for (iii = 0; argv[1][iii] != '\0'; iii++) {
            cfile[iii] = argv[1][iii];
        }
        cfile[iii] = '\0';
        
        if (argc >= 3) {
            if (argv[2][0] == '-' && argv[2][1] == 'd') {
                print_diagnostics = 1;
            }
        }
    }
    else {
        printf("Enter a file [-d] to execute, or Q to quit\n");
        scanf(" %[^\n]", cfile);
        while (getchar() != '\n') ;
    }
    
    if (cfile[0] == 'Q' && cfile[1] == '\0') {
        return 0;
    }
    
    int ccount;
    for (ccount = 0; 1; ccount++) {
        if (cfile[ccount] == '.' && ccount != 0) {
            break;
        }
        if (cfile[ccount] == '\0') {
            halt = 1;
            break;
        }
    }
    if (halt) {
        printf("Invalid file extension.\n");
        if (argc < 2) {
            goto ask_for_file;
        }
        else {
            return 0;
        }
    }
    
    if (cfile[ccount + 1] != 'b' || cfile[ccount + 2] != 'f') {
        printf("Invalid file extension.\n");
        if (argc < 2) {
            goto ask_for_file;
        }
        else {
            return 0;
        }
    }
    ccount += 3;
    int flgcount;
    for (flgcount = ccount; cfile[flgcount] != '\0'; flgcount++) {
        if (cfile[flgcount] == '-' && cfile[flgcount + 1] == 'd') {
            print_diagnostics = 1;
            cfile[ccount] = '\0';
            break;
        }
    }
    
    FILE* file = fopen(cfile, "r");
    if (!file) {
        printf("File failed loading.\n");
        if (argc < 2) {
            goto ask_for_file;
        }
        else {
            return 0;
        }
    }
    
    if (print_diagnostics) printf("Parsing...\n");
    bf_command* prgm = load(file);
    fclose(file);
    if (!prgm) {
        printf("File failed loading.\n");
        if (argc < 2) {
            goto ask_for_file;
        }
        else {
            return 0;
        }
    }
    while (halt) {
        printf("There was an error parsing the program. Continue anyway? (Y/N)");
        scanf(" %c ", cfile);
        if (cfile[0] != 'N' && cfile[0] != 'Y'){
            continue;
        }
        if (cfile[0] == 'N') {
            return 1;
        }
        else halt = 0;
    }
    
    int i = 0, j = 0;
    int count = 0;
    int commands_read = 0;
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
                printf("This information was parsed:\n");
                printprogram(prgm);
                printf("\n");
                free(prgm);
                if (argc < 2) {
                    goto ask_for_file;
                }
                else {
                    return 0;
                }
            }
        }
        if (prgm[i].type == bf_close && prgm[i].val == -1) {
            printf("Error: Mismatched brackets.\n");
            printf("This information was parsed:\n");
            printprogram(prgm);
            printf("\n");
            free(prgm);
            if (argc < 2) {
                goto ask_for_file;
            }
            else {
                return 0;
            }
        }
        if (prgm[i].type == bf_add || prgm[i].type == bf_subtract ||
            prgm[i].type == bf_right || prgm[i].type == bf_left) {
            commands_read += prgm[i].val;
        }
        else if(prgm[i].type != bf_print) commands_read++;
        i++;
    }
    
    if (print_diagnostics){
        printf("Success! %d BF characters read, condensed into %d commands.\n", commands_read, i);
        printprogram(prgm);
        printf("\n");
        printf("Running...\n");
    }
    if (argc == 1) printf("\n");
    run(prgm);
    if (argc == 1) {
        printf("\n");
        printf("Program has terminated successfully! Press enter to continue... \n");
        scanf("%*[^\n]%*c");
    }
    if (print_diagnostics) {
        printf("%d total BF commands run, or %d converted commands.\n", bf_run, cmd_run);
    }
    if (argc == 1) {
        goto ask_for_file;
    }
    return 0;
}

bf_command* load(FILE* file){
    bf_command* prgm = NULL;
    bf_command* tmp;
    int end = 0;
    int num = 0;
    int current = -1;
    char c = getc(file);
    char last = 'A';
    while (!end && !halt) {
        switch (c) {
            case EOF:
                end = 1;
                break;
            
            case '+':
                if (last == c) {
                    prgm[current].val++;
                }
                else {
                    tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", num);
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        num++;
                        current = num - 1;
                        prgm[current].type = bf_add;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case '-':
                if (last == c) {
                    prgm[current].val++;
                }
                else {
                    tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", num);
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        num++;
                        current = num - 1;
                        prgm[current].type = bf_subtract;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case '>':
                if (last == c) {
                    prgm[current].val++;
                }
                else {
                    tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", num);
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        num++;
                        current = num - 1;
                        prgm[current].type = bf_right;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case '<':
                if (last == c) {
                    prgm[current].val++;
                }
                else {
                    tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                    if (!tmp) {
                        printf("Parser error! Ran out of memory at command %d.\n", num);
                        prgm[current].type = bf_end;
                        halt = 1;
                    }
                    else {
                        prgm = tmp;
                        num++;
                        current = num - 1;
                        prgm[current].type = bf_left;
                        prgm[current].val = 1;
                    }
                }
                break;
                
            case ',':
                tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                if (!tmp) {
                    printf("Parser error! Ran out of memory at command %d.\n", num);
                    prgm[current].type = bf_end;
                    halt = 1;
                }
                else {
                    prgm = tmp;
                    num++;
                    current = num - 1;
                    prgm[current].type = bf_input;
                    prgm[current].val = -1;
                }
                break;
                
            case '.':
                tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                if (!tmp) {
                    printf("Parser error! Ran out of memory at command %d.\n", num);
                    prgm[current].type = bf_end;
                    halt = 1;
                }
                else {
                    prgm = tmp;
                    num++;
                    current = num - 1;
                    prgm[current].type = bf_output;
                    prgm[current].val = -1;
                }
                break;

            case '[':
                tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                if (!tmp) {
                    printf("Parser error! Ran out of memory at command %d.\n", num);
                    prgm[current].type = bf_end;
                    halt = 1;
                }
                else {
                    prgm = tmp;
                    num++;
                    current = num - 1;
                    prgm[current].type = bf_open;
                    prgm[current].val = -1;
                }
                break;
                
            case ']':
                tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                if (!tmp) {
                    printf("Parser error! Ran out of memory at command %d.\n", num);
                    prgm[current].type = bf_end;
                    halt = 1;
                }
                else {
                    prgm = tmp;
                    num++;
                    current = num - 1;
                    prgm[current].type = bf_close;
                    prgm[current].val = -1;
                }
                break;
            case '~':
                tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
                if (!tmp) {
                    printf("Parser error! Ran out of memory at command %d.\n", num);
                    prgm[current].type = bf_end;
                    halt = 1;
                }
                else {
                    prgm = tmp;
                    num++;
                    current = num - 1;
                    prgm[current].type = bf_print;
                    prgm[current].val = -1;
                }
                break;
            default:
                break;
        }
        
        last = c;
        c = getc(file);
    }
    
    if (!halt) {
        tmp = realloc(prgm, (num + 1) * sizeof(bf_command));
        if (!tmp) {
            printf("Parser error! Ran out of memory at command %d.\n", num);
            prgm[current].type = bf_end;
            halt = 1;
        }
        else {
            prgm = tmp;
            num++;
            current = num - 1;
            prgm[current].type = bf_end;
            prgm[current].val = 1;
        }
    }
    
    return prgm;
}

void run(bf_command* prgm){
    int ptr;
    for (ptr = 0; ptr < 30000; ptr++) {
        RAM[ptr] = 0;
    }
    ptr = 0;
    int current = 0;
    while (prgm[current].type != bf_end) {
        switch (prgm[current].type) {
            case bf_add:
                RAM[ptr] += prgm[current].val;
                if (print_diagnostics) {bf_run += prgm[current].val; cmd_run++;}
                break;
                
            case bf_subtract:
                RAM[ptr] -= prgm[current].val;
                if (print_diagnostics) {bf_run += prgm[current].val; cmd_run++;}
                break;
                
            case bf_right:
                ptr += prgm[current].val;
                if (print_diagnostics) {bf_run += prgm[current].val; cmd_run++;}
                if (ptr > 29999) {
                    ptr = 29999;
                }
                break;
                
            case bf_left:
                ptr -= prgm[current].val;
                if (print_diagnostics) {bf_run += prgm[current].val; cmd_run++;}
                if (ptr < 0) {
                    ptr = 0;
                }
                break;
                
            case bf_output:
                putchar(RAM[ptr]);
                if (print_diagnostics) {bf_run++; cmd_run++;}
                break;
                
            case bf_input:
                RAM[ptr] = getchar();
                if (print_diagnostics) {bf_run++; cmd_run++;}
                break;
                
            case bf_open:
                if (!RAM[ptr]) {
                    current = prgm[current].val;
                }
                if (print_diagnostics) {bf_run++; cmd_run++;}
                break;
                
            case bf_close:
                if (RAM[ptr]) {
                    current = prgm[current].val;
                }
                if (print_diagnostics) {bf_run++; cmd_run++;}
                break;
                
            case bf_print:
                if (print_diagnostics) {
                    int i;
                    printf("\n(DEBUG INFO) Tape: [");
                    for (i = 0; i <= ptr; i++) {
                        printf(" %d", RAM[i]);
                    }
                    printf(" ] (END)\n");
                }
                break;
            default:
                break;
        }
        current++;
    }
}

void printprogram(bf_command* prgm){
    int i = 0, count;
    while (prgm[i].type != bf_end) {
        switch (prgm[i].type) {
            case bf_add:
                for (count = 0; count < prgm[i].val; count++) {
                    printf("+");
                }
                break;
            case bf_subtract:
                for (count = 0; count < prgm[i].val; count++) {
                    printf("-");
                }
                break;
            case bf_right:
                for (count = 0; count < prgm[i].val; count++) {
                    printf(">");
                }
                break;
            case bf_left:
                for (count = 0; count < prgm[i].val; count++) {
                    printf("<");
                }
                break;
            case bf_input:
                printf(",");
                break;
            case bf_output:
                printf(".");
                break;
            case bf_open:
                printf("[");
                break;
                
            case bf_close:
                printf("]");
                break;
                
            default:
                break;
        }
        i++;
    }
}