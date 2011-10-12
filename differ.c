#include "differ.h"
#include "entry_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>


// Determina diferencias de fecha, hora y tamaño 
// de los archivos del servidor con respecto a 
// estos mismos despues de transcurrir un lapso 
// de tiempo t.
enum diff_type diff(fileEntry e)
{
  ENTRY *found = NULL;
  int gle;
  // Construcción de estructura (key,data) que se inserta 
  // en tabla de hash.
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
      if ((gle = strcmp (found->data, e.date)) == 0)
        result = NODIFF;
      else 
        result = CHANGE;
    }
  else  // Archivo no existe
      result = NEW;

  free (key_str);
  free (qry->key);
  free (qry->data);
  free (qry);
  return result;
}


// Función que construye lista de archivos que presentan 
// diferencias.
entry_node* differ(entry_node* entries){
  entry_node* curr = entries;//Variable de iteracion

  //Lista de diferencias
  entry_node* diffs = NULL;
  for(curr; curr != NULL; curr = curr->next){
    
    //Calcular tipo de diferencia
    enum diff_type dt = diff(curr->e);

    //Si existe una diferencia, agregar la entrada
    if(dt != NODIFF){
      fileEntry e = curr->e; e.dt = dt;
      diffs = add_head(e,diffs);
    }
  }
  return diffs;
}
