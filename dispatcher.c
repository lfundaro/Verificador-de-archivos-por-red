#include "dispatcher.h"
#include <stdlib.h>
#include <stdio.h>

void
dispatcher(URL* url_list){
  int nurls = 0;

  char** pgs = fetcher(url_list,&nurls);
  printf("FETCHER DONE\n");

  entry_node* entries = parser(pgs,nurls);
  printf("PARSER DONE\n");

  entry_node* diffs = differ(entries);
  printf("DIFFER DONE\n");

  updater(diffs);
  printer(diffs);
  free_list(diffs);
  return;
}
