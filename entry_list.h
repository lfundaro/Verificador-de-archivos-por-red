#ifndef ENTRY_LIST_H
#define ENTRY_LIST_H

#include "data_structures.h"

//Agrega un nodo despues del nodo apuntado por el segundo argumento
entry_node* add_entry(struct entry,struct entry_node*);

//Agrega un nodo antes del nodo apuntado por el segundo argumento
entry_node* add_head(struct entry,struct entry_node*);

//Libera el nodo que esta despues del nodo apuntado por el argumento
entry_node* del_entry(struct entry_node*);

//Libera el nodo apuntado por el argumento
entry_node* del_head(struct entry_node*);

void free_list(struct entry_node*);

#endif
