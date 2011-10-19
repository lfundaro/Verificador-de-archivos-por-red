#ifndef PRINTER_H
#define PRINTER_H

#include "data_structures.h"

/** \file */

/**

   Imprime por consola si existe un nuevo archivo o 
   si se modificó un archivo.
   
   \brief Imprime modificaciones en directorio.
   
   \param e. Estructura que contiene información
   de archivos. Se usa para saber si un archivo ha sido 
   marcado con el atributo NODIFF, CHANGE o NEW.
   
   \return No retorna valor.
*/

void print(struct fileEntry);

/**

   Lanzador de acciones print
   
   \brief Llamador de función print.
   
   \param diffs. Lista enlazada con los 
   archivos que han sido modificados o con los nuevos 
   archivos que han sido agregados al directorio. 

   \return No tiene valor de retorno. Realiza operaciones
   de E/S.
*/
void printer(entry_node*);

#endif
