#include "updater.h"
#include "printer.h"
#include <search.h>
#include <stdlib.h>
#include <string.h>

// Actualiza tabla de hash en caso de encontrar un archivo nuevo
// o una modificación.
void update(struct fileEntry e){
  if (e.dt == NODIFF)
    return;
  else 
    {
      // Preparar estructura ENTRY para tabla de hash.
      ENTRY *p = (ENTRY *) smalloc (sizeof(ENTRY));
      int keylength = strlen (e.URL) + strlen (e.path) + 1;
      char *key_str = (char *) smalloc (sizeof (char) * keylength);
      strcpy (key_str, e.URL);
      strcat (key_str, e.path);
      p->key = strdup (key_str);
      p->data = strdup (e.date);
      if (e.dt == NEW)  // NUEVA ENTRADA
          hsearch (*p, ENTER);
      else // MODIFICAR ENTRADA
        {
          ENTRY *pmodif = hsearch (*p, FIND);
          strcpy (pmodif->data, e.date);
        }
      // Liberar recursos
      /* free (p->data); */
      /* free (p->key); */
      /* /\* free (p); *\/ */
      free (key_str);
    }
  return;
}

// Llama a función update
void updater(entry_node* diffs){
  entry_node* curr = diffs;//Variable de iteracion

  //Iterar sobre las diferencias
  for(curr; curr != NULL; curr = curr->next){    
    update(curr->e);
  }
  return;
}
