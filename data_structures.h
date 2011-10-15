#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <search.h>

/** \file */

/**
   Representa el tipo de diferencia de un archivo con respecto
   a lo registrado en la última revisión.
   
   Tipo de diferencia de un archivo.
*/
enum diff_type{NEW , CHANGE , NODIFF};

/**
   Representa una entrada de archivo en el servidor HTTP. Incluye un campo
   para el tipo de diferencia debido a que le es útil a las funciones update(),
   diff() y printer().
   
   \brief Representa una entrada de archivo en el servidor HTTP.
*/
struct fileEntry{

  /** El path dentro del servidor donde se encuentra esta entrada */
  char* path;

  /** El dominio del servidor. Incluye el 'http://'. No incluye el primer '/'*/
  char* URL;

  /** La fecha de modificación de la entrada */
  char* date;

  /** El tamaño del archivo */
  char* size;

  /*Porque cuando 'differ' devuelve entradas,
    debe almacenar que tipo de diferencia era*/
  /** El tipo de diferencia del archivo con respecto al chequeo anterior  */
  enum diff_type dt;

} typedef fileEntry;

struct entry_node typedef entry_node;
struct entry_node{
  fileEntry e;
  entry_node* next;
};


// La estructura URL contiene los siguientes elementos:
//     - dir: almacena el nombre del directorio con URL completo.
//            Ejemplo: http://example.com/dir1              
//     - domain: almacena el dominio del URL. (example.com)
//     - netInfo: almacena la direcciÃ³n IP que corresponde al dominio
//     - next: prÃ³ximo elemento de la lista URL
struct URL typedef URL;

struct URL
{
  char *dir;
  char *domain;
  struct addrinfo *netInfo;
  struct URL *next;
};

// Lista enlazada de apuntadores que se usan 
// para consultar o insertar un elemento en 
// la tabla de hash.
struct eControl typedef eControl;

struct eControl
{
  ENTRY *node;
  eControl *next;
};

// La estructura workerInfo se utiliza para solucionar 
// la limitante que tienen las funciones threads en C 
// las cuales se les pueden pasar un Ãºnico argumento.
struct workerInfo typedef workerInfo;

struct workerInfo 
{
  unsigned int time;
  struct URL* urlList;
  eControl *controlNodes;
};


#endif
