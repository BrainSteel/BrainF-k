/* 
 Brainf>>k
 BF interpreter written by @BrainSteel at PPCG.SE
 http://codegolf.stackexchange.com/users/31054/brainsteel
 
 Usage:
 
 bf [-d] [-h] [-L <language>] [-P <print mode>] <file>
 
 8/20/15 1.3.1
 Added ??? to the language list
 Added -P flag and print modes : char, num, hex, smart
 
 8/20/15 1.3.0
 General Parser implementation
 Code restructure
 Usage improved
 Various minor fixes and changes
 
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
#include "string.h"

#include "common.h"
#include "parse.h"

int run(bf_command* prgm, int debug, void (*printchar)(char byte));

void printchar(char byte);
void printnum(char byte);
void printhex(char byte);
void printsmart(char byte);

int main(int argc, const char * argv[]) {
    /* Default settings */
    char* cfile = NULL;
    ParseFunction prse = ParseBF;
    PrintFunction prnt = PrintBF;
    void (*printmode)(char) = NULL;
    int print_diagnostics = 0;
    
    /* We no longer support an on-the-fly interpreter (interactive mode may be in the works) */
    if (argc < 2) {
        printf("Brainf>>k Interpreter V%d.%d.%d\n", VERSION_MAJOR, VERSION_PATCH, VERSION_TEST);
        printf("Usage: bf [-dh] [-L <language>] [-P <print mode>] <file>\n");
        return 0;
    }
    else {
        /* Parse the options */
        int arg;
        for (arg = 1; arg < argc; arg++) {
            /* Options should start with '-' otherwise, there is no order in the world... */
            if (argv[arg][0] == '-') {
                switch (argv[arg][1]) {
                    case 'd' :
                        print_diagnostics = 1;
                        break;
                    
                    case 'h' :
                        printf("Brainf>>k Interpreter V%d.%d.%d\n", VERSION_MAJOR, VERSION_PATCH, VERSION_TEST);
                        printf("Usage: bf [-dh] [-L <language>] [-P <print mode>] <file>\n");
                        printf("Options:\n");
                        printf("\t-d : Enable debugging symbols and print diagnostics\n");
                        printf("\t-h : Print help\n");
                        printf("\t-L <language> : Sets the language to interpret (BF is default)\n\t\tValid languages:"
#define X(id, name) " " name
                               X_LANGUAGES
#undef X
                               "\n");
                        printf("\t-P <print mode> : Sets the print mode for output (char is default)\n");
                        printf("\t\tchar : Print bytes as characters\n");
                        printf("\t\tnum : Print bytes as space-separated numerical values\n");
                        printf("\t\thex : Print bytes as space-separated hexadecimal values\n");
                        printf("\t\tsmart : Print bytes as characters if printable, as hex otherwise\n");
                        return 0;
                        break;
                     
                    case 'L' :
                        if (arg >= argc - 2) {
                            printf("Error: No language specified.\n");
                            return 0;
                        }
                        else {
                            arg++;
                            /* This is a bit hacky, but we need a leading "if" for "else if" */
                            if (argv[arg] == NULL) {return PARSE_MEMORY;}
#define X(id, name) else if(strcmp(argv[arg], name) == 0) prse = Parse##id, prnt = Print##id;
                            X_LANGUAGES
#undef X                    
                            else {
                                printf("Error: Unidentified language.\n");
                                return 0;
                            }
                        }
                        break;
                        
                    case 'P' :
                        if (arg >= argc - 2) {
                            printf("Error: No print mode specified.\n");
                            return 0;
                        }
                        else {
                            arg++;
                            if (strcmp(argv[arg], "char") == 0) {
                                printmode = printchar;
                            }
                            else if (strcmp(argv[arg], "num") == 0) {
                                printmode = printnum;
                            }
                            else if (strcmp(argv[arg], "hex") == 0) {
                                printmode = printhex;
                            }
                            else if (strcmp(argv[arg], "smart") == 0) {
                                printmode = printsmart;
                            }
                            else {
                                
                            }
                        }
                        break;

                    default:
                        printf("Error: Unidentified argument. Use -h for usage details.\n");
                        return 0;
                }
            }
            else {
                /* This *should* be the last argument */
                cfile = (char*)argv[arg];
            }
        }
    }
    int err, num, read;
    
    if (print_diagnostics) {
        printf("Parsing...\n");
    }
    
    bf_command* prgm = (*prse)(cfile, &num, &read, &err);
    if (err) {
        if (prgm) {
            printf("Information parsed:\n");
            (*prnt)(NULL, prgm);
            free(prgm);
        }
        printf("PARSING ENDED WITH ERROR CODE %d\n", err);
        return err;
    }
    
    if (print_diagnostics){
        printf("Success! %d BF characters read, condensed into %d VM commands.\n", read, num);
        (*prnt)(NULL, prgm);
        printf("\n");
        printf("Running...\n");
    }
    if (argc == 1) printf("\n");
    int vm_run = run(prgm, print_diagnostics, printmode);
    if (print_diagnostics) {
        printf("%d total VM commands run.\n", vm_run);
    }
    return 0;
}

int run(bf_command* prgm, int debug, void (*printchar)(char byte)){
    int ptr;
    int cmd_run;
    char RAM[30000];
    for (ptr = 0; ptr < 30000; ptr++) {
        RAM[ptr] = 0;
    }
    ptr = 0;
    int current = 0;
    while (prgm[current].type != bf_end) {
        switch (prgm[current].type) {
            case bf_add:
                RAM[ptr] += prgm[current].val;
                cmd_run++;
                break;
                
            case bf_subtract:
                RAM[ptr] -= prgm[current].val;
                cmd_run++;
                break;
                
            case bf_right:
                ptr += prgm[current].val;
                cmd_run++;
                if (ptr > 29999) {
                    ptr = 29999;
                }
                break;
                
            case bf_left:
                ptr -= prgm[current].val;
                cmd_run++;
                if (ptr < 0) {
                    ptr = 0;
                }
                break;
                
            case bf_output:
                if (printchar)
                    (*printchar)(RAM[ptr]);
                else putchar(RAM[ptr]);
                cmd_run++;
                break;
                
            case bf_input:
                RAM[ptr] = getchar();
                cmd_run++;
                break;
                
            case bf_open:
                if (!RAM[ptr]) {
                    current = prgm[current].val;
                }
                cmd_run++;
                break;
                
            case bf_close:
                if (RAM[ptr]) {
                    current = prgm[current].val;
                }
                cmd_run++;
                break;
                
            case bf_debug:
                if (debug) {
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
    return cmd_run;
}

void printchar(char byte) {
    printf("%c", byte);
}

void printnum(char byte) {
    printf("%d ", byte);
}

void printhex(char byte) {
    printf("%x ", byte);
}

void printsmart(char byte) {
    if (byte >= 32 && byte < 127) {
        printf("%c", byte);
    }
    else {
        printf("%x", byte);
    }
}


