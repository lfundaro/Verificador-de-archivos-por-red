#include "printer.h"
#include <stdlib.h>
#include <stdio.h>

void print(struct entry e){
  if(e.dt == NEW)
    printf("NEW entry!!!\n");
  else if(e.dt == CHANGE)
    printf("CHANGED entry!!!\n");
  else
    printf("UNCHANGED entry!!!\n");

  return;
}

void printer(entry_node* diffs){
  entry_node* curr = diffs;//Variable de iteracion

  //Iterar sobre las diferencias
  for(curr; curr != NULL; curr = curr->next){    
    print(curr->e);
  }

  return;
}
