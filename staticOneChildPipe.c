#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char ** argv[]){
  int tub[2], tam, state;
  pid_t id;
  char crt[10];
  pipe(tub); // si devuelve 0, todo bien, -1, error
  id = fork();
  if (id == -1)
  { // error
    printf("problemas");
  }
  else
  {
    if (id == 0)
    { // hijo
      close(tub[0]);
      write(tub[1], "hola mundo", 10);
      printf("hijo [%d] envia mensaje \n", getpid());
      close(tub[1]);
      exit(20);
    }
    else
    { // padre
      close(tub[1]);
      waitpid(id, &state, NULL);
      tam = read(tub[0], crt, sizeof(crt));
      printf("Padre recibiendo de hijo [%d] : Mensaje [%s], Tamaño [%d] --- cod: [%d] \n", id, crt, tam, WEXITSTATUS(state));
      close(tub[0]);
    }
  }
}