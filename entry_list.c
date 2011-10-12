#include "entry_list.h"
#include <stdlib.h>

//Agrega un nodo despues del nodo apuntado por el segundo argumento
entry_node* add_entry(struct fileEntry e, struct entry_node* en){
  entry_node* new_en = (entry_node*)smalloc(sizeof(entry_node));
  
  new_en->e = e;
  new_en->next = en->next;
  en->next = new_en;

  return new_en;
}

//Agrega un nodo antes del nodo apuntado por el segundo argumento
entry_node* add_head(struct fileEntry e, struct entry_node* en){
  entry_node* new_en = (entry_node*)smalloc(sizeof(entry_node));
  
  new_en->e = e;
  new_en->next = en;

  return new_en;
}

//Libera el nodo que esta al frente del nodo apuntado por el argumento
entry_node* del_entry(struct entry_node* en){
  entry_node* n = en->next;
  en->next = n->next;
  free(n);

  return en->next;
}

//Libera el nodo apuntado por el argumento
entry_node* del_head(struct entry_node* en){
  entry_node* n = en->next;
  free(en);

  return n;
}

void
free_list(struct entry_node* en){
  entry_node *kill = en;
  entry_node *next = NULL;
  
  while(kill != NULL){

    free_node(kill);

    next = kill->next;
    free(kill);
    kill = next;
  }

  return;
}

void
free_node(struct entry_node* en){
  if(en->e.path != NULL)
    free(en->e.path);

  if(en->e.URL != NULL)
    free(en->e.URL);

  if(en->e.date != NULL)
    free(en->e.date);

  if(en->e.size != NULL)
    free(en->e.size);

  return;
}
