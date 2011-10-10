#include "printer.h"
#include <stdlib.h>
#include <stdio.h>

void print(struct fileEntry e)
{
  if (e.dt == NODIFF)
    return;
  else
    {
      if (e.dt == NEW)
        printf ("Se encontró nuevo archivo: %s\n", e.name);
      else // Archivo cambio
        printf ("Archivo modificado: %s\n", e.name);
    }
  return;
}

void printer(entry_node* diffs){
  entry_node* curr = diffs;//Variable de iteracion

  //Iterar sobre las diferencias
  for(curr; curr != NULL; curr = curr->next)
    print(curr->e);
  return;
}
