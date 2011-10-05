#include "entry_list.h"
#include <stdlib.h>

//Agrega un nodo despues del nodo apuntado por el segundo argumento
entry_node* add_entry(struct entry e, struct entry_node* en){
  entry_node* new_en = (entry_node*)malloc(sizeof(entry_node));
  
  new_en->e = e;
  new_en->next = en->next;
  en->next = new_en;

  return new_en;
}

//Agrega un nodo antes del nodo apuntado por el segundo argumento
entry_node* add_head(struct entry e, struct entry_node* en){
  entry_node* new_en = (entry_node*)malloc(sizeof(entry_node));
  
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

void free_list(struct entry_node* en){
  entry_node *kill = en;
  entry_node *next = NULL;
  
  while(next != NULL){
    kill = next;
    next = kill->next;
    free(kill);
  }
}
