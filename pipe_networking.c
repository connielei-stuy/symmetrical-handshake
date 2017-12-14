#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo("wkp", 0644); //creates FIFO (well known pipe)
  int upstream = open("wkp", O_RDONLY); //opens FIFO and waits for connection
  
  char hand[HANDSHAKE_BUFFER_SIZE];
  read(upstream, hand, sizeof(hand)); //reads client's private FIFO name
  remove("wkp"); //removes well known pipe so another client doesn't connect
   
  int downstream = open(hand, O_WRONLY); //connects to client's private FIFO
  write(downstream, ACK, sizeof(ACK)); //sends initial message after connecting
  read(upstream, hand, sizeof(hand));

  *to_client = downstream; //add comment here to remind self about addresses, pointers
  return upstream;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char hand[HANDSHAKE_BUFFER_SIZE];
  //itoa(getpid(), hand, 10);

  sprintf(hand, "%d", getpid());
  mkfifo(hand, 0644); //creates private FIFO

  int upstream = open("wkp", O_WRONLY); //connects to server's WKP
  if(upstream == -1){
    printf("server not running\n");
    remove(hand);
    exit(0);
  }
  write(upstream, hand, sizeof(hand)); //sends private FIFO name to server

  int downstream = open(hand, O_RDONLY); //opens private FIFO

  char ack[sizeof(ACK)];
  read(downstream, ack, sizeof(ack)); //waits for server acknowledgement message
  remove(hand);
  write(upstream, ACK, sizeof(ACK));

  *to_server = upstream;

  return downstream;
}
