#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned int run(const char* code, unsigned int size){
    unsigned short ptr = 0;
    unsigned char* memory = (unsigned char*) malloc(USHRT_MAX+1);
    unsigned int stack[256];
    unsigned char sp = 0;
    if (memory == NULL){
        fputs("\nBrainFuck memory allocate error!", stderr);
        return 1;
    }
    for(unsigned int i = 0; i < size; ++i){
        switch (code[i]){
            case '<':
                --ptr;
                break;
            case '>':
                ++ptr;
                break;
            case '+':
                ++memory[ptr];
                break;
            case '-':
                --memory[ptr];
                break;
            case '.':
                putchar(memory[ptr]);
                break;
            case ',':
                memory[ptr] = getchar();
                break;
            case '[':
                if (memory[ptr] == 0){
                    for(; i < size; ++i){
                        if (code[i] == ']'){
                                break;
                        }
                    }
                }
                else{
                    if (sp == UCHAR_MAX){
                        fprintf(stderr, "\nBrainFuck stack overflow in %u symbol\n", i);
                        free(memory);
                        return 3;
                    }
                    stack[sp] = i;
                    ++sp;
                }
                break;
            case ']':
                if (sp == 0){
                    fprintf(stderr, "\nBrainFuck error: ']' without '[' in %u symbol\n", i);
                    free(memory);
                    return 2;
                }
                if (memory[ptr] != 0){
                    i = stack[sp-1];
                }
                else{
                    --sp;
                }
                break; 
            default:
                break;
        }
    }
    free(memory);
    fflush(stdout);
    return 0;
}
int main(int argc, char** argv){
    if (argc != 2){
        fputs("Uncorrect arguments! Use -h for help\n", stderr);
        return 1;
    } if (*argv[1] == '-'){
        if(argv[1][1] == 'h'){
            fputs("BrainFuck interpretator:\n-d - doc\n-v - info\nUse:\nbf file.bf\n", stderr);
        }
        else if(argv[1][1] == 'd'){
            fputs("Brainfuck documentation:\nEng: https://en.wikipedia.org/wiki/Brainfuck\nRus: https://ru.wikipedia.org/wiki/Brainfuck\nThis interpretator:\nMemory size: ~65KB\nStack size: 256\nMax programm size: ~4GB\n", stderr);
        }
        else if(argv[1][1] == 'v'){
            fputs("BrainFuck interpreter 1.0\nAuthor: OriginTravel(@github)\nLicense: GNU GPL v3.0\n", stderr);
        }
        else{
            fputs("Uncorrect Flag! Use -h for help\n", stderr);
            return 1;
        }
        return 0;
    }
    FILE* fptr = fopen(argv[1], "r");
    if (fptr == NULL){
        fputs("File open error\n", stderr);
        return 1;
    }
    if (fseek(fptr, 0, SEEK_END) == -1){
        fputs("Fseek open error\n", stderr);
        fclose(fptr);
        return 1;
    }
    unsigned n = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char* data = (char*) malloc(n);
    if (data == NULL){
        fputs("Memory allocation error\n", stderr);
        fclose(fptr);
        return 1;
    }
    unsigned int sum = 0;
    while((sum+=fread(data, 1, n, fptr)) != n){}
    fclose(fptr);
    sum = (unsigned int)run(data, n);
    free(data);
    fprintf(stderr, "\nExit code: %u\n", sum);
    return (int)sum;
}
