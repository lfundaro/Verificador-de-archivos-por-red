#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "data_structures.h"
#include "fetcher.h"
#include "parser.h"
#include "differ.h"
#include "printer.h"

/** \file */

/**

   Función que ejecuta el conjunto de operaciones que 
   tiene que hacer el thread worker. Primero se llama 
   a fetcher para descargar las páginas HTML especificadas
   por el usuario. Luego parser extrae la información relevante
   en el/los HTML/s. Luego differ se encarga de determinar 
   las diferencias en fecha, tamaño y hora entre archivos y 
   finalmente printer se encarga de notificar por cónsola los 
   cambios de archivos o si existe un nuevo archivo.

  \brief Orquetador de operaciones.

  \param URL* lista enlazada con información de URL y su dirección
  ip.

  \param eControl** apuntador a lista enlazada que lleva control 
  de los apuntadores que se almacenan en la tabla de hash.

  \return retorna (void). Sin embargo, retorna la estructura 
  eControl** con los apuntadores 
*/
void 
dispatcher(URL* url_list, eControl **controlNodes);

#endif
