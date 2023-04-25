#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

void closePipeNVeces(int index, int** fd, int sw);

int main(int argc, char ** argv){
  int numChild = atoi(argv[1]);
  pid_t *childsId = malloc(numChild * sizeof(numChild));
  int** fd;
  fd = (int **) malloc(numChild*sizeof(int*));
  for (int i = 0; i < numChild; i++){
    fd[i] = (int*) malloc(2*sizeof(int));
    pipe(fd[i]);
    childsId[i] = fork();
    if(childsId[i] == -1){
      printf("Algo salio mal al crear el hijo numero [%d]\n", i);
      exit(1);
    }else if(childsId[i] == 0){
      closePipeNVeces(i + 1, fd, 1);
      char msg[20];
      int bytesLeidos = read(fd[i][0], msg, 20);
      printf("El hijo numero [%d], pid [%d] recibio el sig msg: [%s]\n", i, getpid(), msg);
      close(fd[i][0]);
      close(fd[i][1]);
      exit(EXIT_SUCCESS);
    }else{
      char idChar[10];
      sprintf(idChar, "%d", childsId[i]);
      char msg1[20] = "Hola hijo ";
      strcat(msg1, idChar);
      close(fd[i][0]);
      printf("Envie mensaje a hijo [%d]\n", i);
      write(fd[i][1], msg1, 20);
      close(fd[i][1]);
      waitpid(childsId[i], NULL, 0);
    }
  } 
  return EXIT_SUCCESS;
}
void closePipeNVeces(int index,int** fd,  int sw){
  for (int i = 0; i < index; i++){
    if(sw){ // cerrar escritura
      close(fd[i][1]);
      if(i != index - 1){
        close(fd[i][0]);
      }
    }else{ //cerrar lectura
      close(fd[i][0]);
    }
  }
  
}