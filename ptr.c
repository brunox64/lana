#include <stdio.h>

void testFnPtr(void);
void testFnPtr2(void);
void testSamplePtr(void);
void testFGets(void);
void testGets(void);

void readLine(char *str, int maxLen);
int* sumOnePtrInt(int *ptr);

int main(void) {

    testSamplePtr();

    return 0;
}

void testGets(void) {
    char nome[200];
    printf("digite um valor e tecle enter\n");
    gets(nome);
    printf("valor digitado:%s:\n", nome);
}

void testFGets(void) {
    char nome[100];
    printf("digite um valor e tecle enter\n");
    char *outro = fgets(nome, 10, stdin);
    printf("valor digitado:%s\n", outro);
    printf("valor digitado:%s\n", nome);

    printf("p1: %p, p2: %p\n", nome, outro);
}

void testSamplePtr(void) {
    char nome[100];
    printf("digite um valor e tecle enter\n");
    readLine(nome, 100);
    printf("valor digitado:%s:\n", nome);
}

void testFnPtr(void) {

    char nome[11];

    printf("digite um valor e tecle enter\n");

    // void (*rl)(char *str, int maxLen) = &readLine;
    void (*rl)(char *str, int maxLen) = readLine;

    rl(nome, 11);

    printf("valor digitado:%s:\n", nome);
}

void testFnPtr2(void) {
    
    int* (*rl2)(int *ptr) = sumOnePtrInt;

    int val = 10;
    
    // val = *(rl2(&val));
    rl2(&val);

    printf("val depois:%d\n", val);
}

int *sumOnePtrInt(int *ptr) {
    (*ptr)++;
    return ptr;
}

void readLine(char *str, int maxLen) {
    
    if (maxLen <= 0) {
        printf("valor maxLen deve ser maior que zero!\n");
        return;
    }

    int len = 0;

    while (1) {
        *str = getchar();
        
        if (*str == '\n' || *str == '\0') {
            break;
        }

        len++;

        if (len >= maxLen) {
            break;
        }

        str++;
    }

    *str = '\0';
}