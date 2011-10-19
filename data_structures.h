#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <search.h>

/** \file */

/**
   El tipo enum diff_type sirve para modelar los diferentes 
   estados que pueden tener los archivos: NUEVO, MODIFICADO O 
   SIN MODIFICAR.

  \brief Modela estados de archivos.

  \param NEW, CHANGE, NODIFF. Los diferentes valores con los que 
  se modelan la presencia o ausencia de cambios en archivos.

  \return No tiene valor de retorno.

*/
enum diff_type{NEW , CHANGE , NODIFF};



/**

   Tipo de dato que contiene información acerca de un archivo. 
   Se utiliza para almacenar características que luego son
   usadas en opeaciones de diferencia y actualización.
   
  \brief Estructura que modela las características de un objeto.
  
  \param char* path. La ruta al directorio que se quiere examinar.
  
  \param char* URL. La dirección URL completa, incluyendo el
  dominio.

  \param char* date. La fecha de modificación del archivo.

  \param char* size. El tamaño del archivo.

  \return No tiene valor de retorno.
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


/**

   Lista enlazada de tipos fileEntry.
   
  \brief Estructura que almacena los diferentes fileEntry's 
  del programa.
  
  \return No tiene valor de retorno.
*/


struct entry_node typedef entry_node;
struct entry_node{
  fileEntry e;
  entry_node* next;
};


/**

   Estructura que contiene información sobre las direcciones
   URL que se especifican revisar periódicamente en el programa.
   
  \brief Información sobre URL.
  
  \param char* dir. Directorio que se debe verificar. 
  
  \param char* domain. Dominio presente en el URL. 

  \param addrinfo* netInfo. Dirección IP que corresponde 
  al dominio.

  \param URL* next. Próximo elemento en la lista enlazada 
  de tipos URL. 

  \return No tiene valor de retorno.
*/
struct URL typedef URL;

struct URL
{
  char *dir;
  char *domain;
  struct addrinfo *netInfo;
  struct URL *next;
};

/**

   Lista enlazada de apuntadores que se usan 
   para consultar o insertar un elemento en 
   la tabla de hash.
   
  \brief Estructura que se utiliza para llevar 
  control de estructuras almacenadas en memoria. 
  
  \param ENTRY* node. Nodo de tipo ENTRY. Es una 
  tupla (key, value).

  \return No tiene valor de retorno.
*/

struct eControl typedef eControl;

struct eControl
{
  ENTRY *node;
  eControl *next;
};


/**

   La estructura workerInfo se utiliza para solucionar 
   la limitante que tienen las funciones threads en C 
   las cuales se les pueden pasar un único argumento.
   
  \brief Información útil para hilo.
  
  \param unsigned int time. Hora en la que ha sido 
  modificado un archivo. 

  \param URL* urlList. Lista enlazada de estructuras 
  URL.

  \param eControl* controlNodes. Lista enlazada de 
  estructura que mantiene control de estructuras 
  reservadas en memoria.
  
  \return No tiene valor de retorno.
*/

struct workerInfo typedef workerInfo;

struct workerInfo 
{
  unsigned int time;
  struct URL* urlList;
  eControl *controlNodes;
};


#endif
