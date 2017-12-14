#include "pipe_networking.h"

void piping(int to_server, int from_server){
  char buff[BUFFER_SIZE];
  printf("input: ");
  fgets(buff, BUFFER_SIZE, stdin);
  *strchr(buff, '\n') = 0;

  write(to_server, buff, BUFFER_SIZE);
  read(from_server, buff, BUFFER_SIZE);
  printf("output: %s\n", buff);
}


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while(1){
    piping(to_server, from_server);
  }
}
