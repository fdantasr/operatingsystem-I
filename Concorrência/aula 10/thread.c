#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int counter = 0;

void *meuthread(void *arg)
{
  int i;
  for (i = 0; i < 1e6; i++)
  {
    counter++;
  }
  printf("%d :pronto\n", (char *)arg);
  return NULL;
}

int main()
{
  phtread_t t1, t2;
  printf("Contador Inicial: %d\n", counter);
  pthread_create(&t1, NULL, meuthread, (void *)"A");
  pthread_create(&t2, NULL, meuthread, (void *)"B");

  printf("Contador Final: %d\n", counter);
}