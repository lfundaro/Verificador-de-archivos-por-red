#include "dispatcher.h"
#include <stdlib.h>
#include <stdio.h>

// Función que ejecuta el conjunto de operaciones que 
// tiene que hacer el thread worker. Primero se llama 
// a fetcher para descargar las páginas HTML especificadas
// por el usuario. Luego parser extrae la información relevante
// en el/los HTML/s. Luego differ se encarga de determinar las 
// diferencias en fecha, tamaño y hora entre archivos y finalmente
// printer se encarga de notificar por cónsola los cambios de archivos
// o si existe un nuevo archivo.
void
dispatcher(URL* url_list){
  int nurls = 0;

  //  printf("FETCHER START\n"); //(FLAG)
  // Recuperar información del servidor HTTP
  char** pgs = fetcher(url_list,&nurls);
  //printf("FETCHER DONE\n"); //(FLAG)

  //printf("PARSER START\n"); //(FLAG)
  // Extraer información necesaria del HTML
  entry_node* entries = parser(pgs,nurls);
  //printf("PARSER DONE\n"); //(FLAG)

  //  printf("DIFFER START\n"); //(FLAG)
  // Sacar diferencias entre archivos
  entry_node* diffs = differ(entries);
  //  printf("DIFFER DONE\n"); //(FLAG)

  // Actualizar entradas de tabla de hash 
  updater(diffs);

  // Imprimir resultados
  printer(diffs);

  free_list(diffs);
  return;
}
