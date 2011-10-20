#ifndef ENTRY_LIST_H
#define ENTRY_LIST_H

#include "data_structures.h"

/** \file x */

/**
   Agrega un nodo después del nodo apuntado por el segundo argumento.
   
   \brief Agrega un nodo después del nodo apuntado por el segundo argumento.

   \param e Entrada de archivo a agregar.

   \param l Lista en donde agregar la entrada

   \return Apuntador al comienzo de la lista enlazada.
*/
entry_node* add_entry(struct fileEntry e, struct entry_node* l);

/**
   Agrega un nodo antes del nodo apuntado por el segundo argumento.

   \brief Agrega un nodo antes del nodo apuntado por el segundo argumento.

   \param e Entrada de archivo a agregar.

   \param l Lista en donde agregar la entrada

   \return Apuntador al comienzo de la lista enlazada.
*/
entry_node* add_head(struct fileEntry e, struct entry_node* l);

/**
   Libera el nodo que esta después del nodo apuntado por el argumento

   \brief Libera el nodo que esta después del nodo apuntado por el argumento

   \param l Lista en donde borrar la entrada

   \return Apuntador al comienzo de la lista enlazada.
*/
entry_node* del_entry(struct entry_node* l);

/**
   Libera el nodo apuntado por el argumento

   \brief Libera el nodo apuntado por el argumento

   \param l Lista en donde borrar la entrada

   \return Apuntador al comienzo de la lista enlazada.

*/
entry_node* del_head(struct entry_node* l);

/**
   Libera una lista enlazada y todos sus contenidos
   
   \brief Libera una lista enlazada y todos sus contenidos

   \param l Cabeza de la lista a liberar
*/
void free_list(struct entry_node* l);

/**
   Libera una nodo de una lista enlazada
   
   \brief Libera una nodo de una lista enlazada

   \param l Nodo a liberar   
*/
void free_node(struct entry_node* l);
#endif
