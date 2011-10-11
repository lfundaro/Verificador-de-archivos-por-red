#include "dispatcher.h"
#include <stdlib.h>
#include <stdio.h>

void
dispatcher(URL* url_list){
  int nurls = 0;

  //  printf("FETCHER START\n"); //(FLAG)
  char** pgs = fetcher(url_list,&nurls);
  //printf("FETCHER DONE\n"); //(FLAG)

  //printf("PARSER START\n"); //(FLAG)
  entry_node* entries = parser(pgs,nurls);
  //printf("PARSER DONE\n"); //(FLAG)

  //  printf("DIFFER START\n"); //(FLAG)
  entry_node* diffs = differ(entries);
  //  printf("DIFFER DONE\n"); //(FLAG)

  updater(diffs);
  printer(diffs);
  free_list(diffs);
  return;
}
