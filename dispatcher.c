#include "dispatcher.h"
#include <stdlib.h>
#include <stdio.h>

void
dispatcher(URL* url_list){
  char** pgs = fetcher(/*addrs*/NULL,/*naddrs*/0);
  printf("FETCHER DONE\n");

  entry_node* entries = parser(pgs,/* naddrs */0);
  printf("PARSER DONE\n");

  entry_node* diffs = differ(entries);
  printf("DIFFER DONE\n");

  updater(diffs);
  printer (diffs);
  free_list (diffs);
  return;
}
