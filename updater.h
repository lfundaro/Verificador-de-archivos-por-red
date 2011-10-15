#ifndef UPDATER_H
#define UPDATER_H

#include "data_structures.h"

/** \file */

/**

   Actualiza tabla de hash en caso de encontrar un archivo nuevo
   o una modificación.
   
   \brief Actualiza tabla de hash.
   
   \param fileEntry. Información sobre archivo.

   \param eControl **controlNodes. Lista enlazada de 
   de apuntadores a estructuras en memoria.

   \return No tiene valor de retorno.

*/
void 
update(struct fileEntry,  eControl **controlNodes);


/**
   Llama a función update
   
   \brief Función llamadora a update.
   
   \param entry_node* diffs. Diferencias de archivos.

   \param eControl **controlNodes. Lista enlazada de 
   de apuntadores a estructuras en memoria.

   \return No tiene valor de retorno.
*/
void 
updater(entry_node*, eControl **controlNodes);

#endif
