#include "updater.h"
#include "printer.h"
#include <stdlib.h>

void update(struct entry e){
  return;
}

void updater(entry_node* diffs){
  entry_node* curr = diffs;//Variable de iteracion

  //Iterar sobre las diferencias
  for(curr; curr != NULL; curr = curr->next){    
    update(curr->e);
    print(curr->e);
  }

  //Liberar memoria
  free_list(diffs);

  return;
}
