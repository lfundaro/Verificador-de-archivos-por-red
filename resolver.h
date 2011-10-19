#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "data_structures.h"

/** \file */

/**

   Resuelve direcciones URL a IP.
   Almacena las direcciones IP en la estructura
   netInfo.
   
   \brief Resuelve URL a IP.
   
   \param urlList. Lista enlazada de direcciones
   URL.

   \return No tiene valor de retorno.

*/
void
resolve (URL *urlList);
