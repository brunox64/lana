#include <stdio.h>

void readLine(char *str);

int main() {
    int a = 10;
    int b = 10;
    int c;

    printf("digite um número qualquer\n");
    scanf("%d", &c);

    printf("aqui a um resultado qualquer: %d\n", (a + b + c));

    printf("digite o nome de alguém\n");

    char nome[201];
    
    readLine(nome);

    printf("o valor digitado foi:%s:\n", nome);

    return 0;
}

void readLine(char *str) {
    // gets(nome);
    // fgets(nome, 200, stdin);
    // scanf("%s%[a-z ]", nome, sobrenome);
    // getline(name, 21, stdin);

    int min = 101;
    int max = 1001;
    int total = min+max-1;

    char part1[min];
    char part2[max];

    scanf("%s%[^\n]%*c", part1, part2);

    char val[total];
    int index = -1;

    for (int i = 0; i < min; i++) {
        if (part1[i] == '\0') {
            break;
        }

        index++;
        val[index] = part1[i];
    }

    for (int i = 0; i < max; i++) {
        if (part2[i] == '\0') {
            break;
        }

        index++;
        val[index] = part2[i];
    }

    index++;
    val[index] = '\0';

    printf("nome aqui: %s\n", val);

    for (int i = 0; i < total; i++) {
        *str++ = val[i];

        if (val[i] == '\0') {
            break;
        }
    }
}

