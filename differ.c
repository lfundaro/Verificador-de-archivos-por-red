#include "differ.h"
#include "entry_list.h"
#include <stdlib.h>
#include <stdio.h>

enum diff_type diff(entry e){
  return NEW;//Retornar diff de tipo NEW
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
      entry e = curr->e; e.dt = dt;
      diffs = add_head(e,diffs);
    }
  }

  //Liberar memoria
  free_list(entries);

  return diffs;
}
