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
  ENTRY *qry = (ENTRY *) malloc (sizeof(ENTRY));
  int keylength = strlen (e.URL) + strlen (e.path) + 1;
  char *key_str = (char *) malloc (sizeof (char) * keylength);
  strcpy (key_str, e.URL);
  strcat (key_str, e.path);
  qry->key = strdup (key_str);
  qry->data = strdup (e.date);
  enum diff_type result;
  
  // Chequear si es un nuevo archivo 
  if ((found = hsearch (*qry, FIND)) != NULL) // Archivo existe
    {
      free (qry->key);
      if ((gle = strcmp (found->data, e.date)) == 0)
        result = NODIFF;
      else 
        result = CHANGE;
    }
  else  // Archivo no existe
      result = NEW;
  free (key_str);
  free (qry);
  return result;
}

entry_node* differ(entry_node* entries){
  entry_node* curr = entries;//Variable de iteracion

  //Lista de diferencias
  entry_node* diffs = NULL;
  for(curr; curr != NULL; curr = curr->next){
    
    //Calcular tipo de diferencia
    enum diff_type dt = diff(curr->e);
    //    printf("Calculated diff: %d\n",dt); (FLAG)

    //Si existe una diferencia, agregar la entrada
    if(dt != NODIFF){
      //      printf("changed detected: %d\n",dt); (FLAG)
      fileEntry e = curr->e; e.dt = dt;
      diffs = add_head(e,diffs);
    }
  }

  return diffs;
}
