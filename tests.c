#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

#include "fetcher.h"
#include "parser.h"

int
main(){
  int ret = 0; //to hold return values
  int i = 0; //iteration variable

  unsigned short naddrs = 1;
  struct addrinfo* addrs = (struct addrinfo*)malloc(sizeof(struct addrinfo)*naddrs);
  for(i = 0; i < naddrs; ++i){
    memset(&(addrs[i]),0,sizeof(struct addrinfo));
  }

  //Initialize hints structure to create addrinfos
  struct addrinfo hints;
  memset(&hints,0,sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  //The protocol is left to the program to decide.
  //Since there aren't any protocols that are actually
  //used for streaming besides TCP, we can leave it
  //at it's default value. The correct behaviour would
  //be to use 'getprotoent'.

  ret = getaddrinfo("localhost","http",&hints,&addrs);;
  if (ret){
    printf("Error getting info about host");
    exit(1);
  }

  char** pgs = fetcher(addrs,naddrs);
  printf("FETCHER DONE\n");

  entry_node* entries = parser(pgs,naddrs);
  printf("PARSER DONE\n");

  return 0;
}
