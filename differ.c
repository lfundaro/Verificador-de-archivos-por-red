#include "differ.h"
#include "entry_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>

enum diff_type diff(fileEntry e)
{
  ENTRY *found = NULL;
  int gle;
  ENTRY qry;
  qry.key = strdup (e.URL);
  qry.data = strdup (e.date);
  
  // Chequear si es un nuevo archivo 
  if ((found = hsearch (qry, FIND)) != NULL) // Archivo existe
    {
      free (qry.key);
      if ((gle = strcmp (found->data, e.date)) == 0)
        return NODIFF;
      else 
        return CHANGE;
    }
  else  // Archivo no existe
      return NEW;
}

entry_node* differ(entry_node* entries){
  entry_node* curr = entries;//Variable de iteracion

  //Lista de diferencias
  entry_node* diffs = NULL;
  for(curr; curr != NULL; curr = curr->next){
    
    //Calcular tipo de diferencia
    enum diff_type dt = diff(curr->e);
    printf("Calculated diff: %d\n",dt);

    //Si existe una diferencia, agregar la entrada
    if(dt != NODIFF){
      printf("changed detected: %d\n",dt);
      fileEntry e = curr->e; e.dt = dt;
      diffs = add_head(e,diffs);
    }
  }

  //Liberar memoria
  free_list(entries);

  return diffs;
}
