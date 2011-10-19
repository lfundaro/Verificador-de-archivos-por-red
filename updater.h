#ifndef UPDATER_H
#define UPDATER_H

#include "data_structures.h"

/** \file */

/**

   Actualiza tabla de hash en caso de encontrar un archivo nuevo
   o una modificación.
   
   \brief Actualiza tabla de hash.
   
   \param e. Información sobre archivo.

   \param controlNodes. Lista enlazada de 
   de apuntadores a estructuras en memoria.

   \return No tiene valor de retorno.

*/
void 
update(struct fileEntry,  eControl **controlNodes);


/**
   Llama a función update
   
   \brief Función llamadora a update.
   
   \param diffs. Diferencias de archivos.

   \param controlNodes. Lista enlazada de 
   de apuntadores a estructuras en memoria.

   \return No tiene valor de retorno.
*/
void 
updater(entry_node*, eControl **controlNodes);

#endif
