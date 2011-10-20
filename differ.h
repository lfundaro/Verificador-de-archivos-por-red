#ifndef DIFFER_H
#define DIFFER_H

#include "data_structures.h"

/** \file */

/**
   Determina diferencias de fecha, hora y tamaño 
   los archivos del servidor con respecto a 
   mismos despues de transcurrir un lapso 
   tiempo t.

  \brief Determina modificación o creación de un archivo.

  \param e. Información sobre arhivo. 

  \return Retorna un indicador de diferencia: NODIFF, CHANGE o NEW.
*/
enum diff_type diff(fileEntry e);

/**
   Función que itera sobre estructura entry_node y para cada 
   elemento llama la función diff.

  \brief Ejecutar función de diferencias.

  \param entries. Apuntador a una lista enlazada de tipos fileEntry. 

  \return entry_node* modificado indicando modificaciones en archivos 
  o nuevos archivos.
*/
entry_node* differ(entry_node* entries);

#endif
