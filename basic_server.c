#include "pipe_networking.h"

void piping(int to_client, int from_client){
  char buff[BUFFER_SIZE];
  read(from_client, buff, BUFFER_SIZE);

  char * letter = buff;
  while( *letter){
    *letter = toupper((unsigned char) * letter);
    letter ++;
  }
  
  write(to_client, buff, BUFFER_SIZE);
}

int main() {

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  
  while(1){
    piping(to_client, from_client);
  }
}
