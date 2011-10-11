#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

#include "data_structures.h"
#include "fetcher.h"
#include "parser.h"
#include "printer.h"

int
main(){
  int ret = 0; //to hold return values
  int i = 0; //iteration variable

  int naddrs = 1;
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

  URL* url_list = (URL*)malloc(sizeof(URL));
  url_list->domain = "http://localhost";
  url_list->dir = "/~jorge/";
  url_list->netInfo = &(addrs[0]);
  url_list->next = NULL;

  char** pgs = fetcher(url_list,&naddrs);
  printf("FETCHER DONE\n");

  entry_node* entries = parser(pgs,naddrs);
  printf("PARSER DONE\n");

  printer(entries);
  printf("PRINTER DONE\n");

  return 0;
}
