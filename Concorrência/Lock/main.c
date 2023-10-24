#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int main(

    lock_t mutex;      // Aqui é declarado o lock
    lock(&mutex);      // Aqui é feito o lock
    saldo = saldo + 1; // Aqui é feita a operação crítica
    unlock = (&mutex); // Aqui é feito o unlock que libera o lock e permite que outra thread execute a operação crítica
    return 0;);