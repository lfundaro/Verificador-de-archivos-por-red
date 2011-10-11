#include "updater.h"
#include "printer.h"
#include <search.h>
#include <stdlib.h>
#include <string.h>


void update(struct fileEntry e){
  if (e.dt == NODIFF)
    return;
  else 
    {
      ENTRY *p = (ENTRY *) malloc (sizeof(ENTRY));
      int keylength = strlen (e.URL) + strlen (e.path) + 1;
      char *key_str = (char *) malloc (sizeof (char) * keylength);
      strcpy (key_str, e.URL);
      strcat (key_str, e.path);
      p->key = strdup (key_str);
      p->data = strdup (e.date);
      if (e.dt == NEW) 
        hsearch (*p, ENTER);
      else // MODIFIY
        {
          ENTRY *pmodif = hsearch (*p, FIND);
          strcpy (pmodif->data, e.date);
        }
      free (p);
      free (key_str);
    }
  return;
}

void updater(entry_node* diffs){
  entry_node* curr = diffs;//Variable de iteracion

  //Iterar sobre las diferencias
  for(curr; curr != NULL; curr = curr->next){    
    update(curr->e);
  }
  return;
}
