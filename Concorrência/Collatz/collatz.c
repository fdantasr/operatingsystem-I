#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SEQUENCE 4096

int sequence[MAX_SEQUENCE];
int size = 0;

void *collatz(void *arg)
{
  int num = *((int *)arg);
  sequence[0] = num;
  size = 1;

  while (num != 1)
  {
    if (num % 2 == 0)
    {
      num = num / 2;
    }
    else
    {
      num = 3 * num + 1;
    }
    sequence[size] = num;
    size++;
  }
  pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
  char buffer[MAX_SEQUENCE];
  printf("Digite um numero: ");
  fgets(buffer, MAX_SEQUENCE, stdin);

  int num = atoi(buffer);
  pthread_t t1;
  pthread_create(&t1, NULL, collatz, &num);
  pthread_join(t1, NULL);

  printf("Sequencia: ");
  for (int i = 0; i < size; i++)
  {
    printf("%d ", sequence[i]);
  }
  printf("\n");

  return 0;
}
