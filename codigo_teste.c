#include <stdio.h>
#include <stdlib.h>

void vetor_estouro() {
    int vetor[5];
    // ERRO 1: Acessando índice 5 (fora do limite 0-4)
    vetor[5] = 10; 
}

void variavel_nao_iniciada() {
    int x;
    // ERRO 2: Usando 'x' sem dar valor inicial
    if (x == 10) {
        printf("X vale 10");
    }
}

void vazamento_memoria() {
    // ERRO 3: Alocando memória e não liberando (sem free)
    char *ptr = (char *)malloc(10 * sizeof(char));
    ptr[0] = 'a';
}

int main() {
    vetor_estouro();
    variavel_nao_iniciada();
    vazamento_memoria();
    return 0;
}