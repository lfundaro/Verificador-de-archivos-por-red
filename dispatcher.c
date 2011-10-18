#include "dispatcher.h"
#include <stdlib.h>
#include <stdio.h>

void
dispatcher(URL* url_list,  eControl **controlNodes)
{
  int nurls = 0;

  // Recuperar información del servidor HTTP
  char** pgs = fetcher(url_list,&nurls);

  // Extraer información necesaria del HTML
  entry_node* entries = parser(pgs,nurls);

  // Sacar diferencias entre archivos
  entry_node* diffs = differ(entries);

  // Actualizar entradas de tabla de hash 
  updater(diffs, controlNodes);

  // Imprimir resultados
  printer(diffs);

  free_list(diffs);
  return;
}
