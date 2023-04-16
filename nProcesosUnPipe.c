#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char **argv){
  int numChld = atoi(argv[1]);
  int fd[2];
  pipe(fd);
  for(int i = 0; i<numChld; i++){
    pid_t childID = fork();
    switch (childID){
      case -1:{
        printf("ERROR al crear un hijo");
        break;
      }
      case 0:{
        char msg[20];
        close(fd[1]);
        int bytesLeidos = read(fd[0], msg, 20);
        printf("soy el hijo #[%d], recibí de mi padre [%s], lo que son [%d] bytes \n", getpid(), msg, bytesLeidos);
        printf("soy el hijo [%d], no he terminado, voy en [%d]\n ", getpid(), i);
        close(fd[0]);
        break;
      }
      
      default:{
        close(fd[0]);
        printf("mensaje enviado de parte del padre a todos los hijos\n");
        write(fd[1],"mensaje de padre\n", 16);
        close(fd[1]);
        printf("soy el padre, no he terminado, voy en [%d]\n ", i);
        exit(0);
        break;
      }
    }
  }
  return EXIT_SUCCESS;
}
