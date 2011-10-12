#include "printer.h"
#include <stdlib.h>
#include <stdio.h>


// Imprime por consola si existe un nuevo archivo o 
// si se modificó un archivo.
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

// Lanzador de acciones print
void 
printer(entry_node* diffs)
{
  entry_node* curr = diffs;//Variable de iteracion

  //Iterar sobre las diferencias
  for(curr; curr != NULL; curr = curr->next)
    print(curr->e);

  return;
}
