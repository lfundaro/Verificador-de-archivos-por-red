#include "printer.h"
#include <stdlib.h>
#include <stdio.h>

void 
print (struct fileEntry e)
{
  if (e.dt == NODIFF)
    return;
  else
    {
      if (e.dt == NEW)
        printf ("Se encontró nuevo archivo: %s%s\n",e.URL, e.path);
      else // Archivo cambio
        printf ("Archivo modificado: %s%s\n", e.URL, e.path);
    }
  return;
}

void 
printer(entry_node* diffs)
{
  entry_node* curr = diffs;//Variable de iteracion

  if(diffs == NULL){
    printf("No se encontraron cambios en el directorio\n\n");
    return;
  }

  //Iterar sobre las diferencias
  for(curr; curr != NULL; curr = curr->next){
    print(curr->e);
  }

  printf("\n");

  return;
}
