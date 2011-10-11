#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

enum diff_type{NEW , CHANGE , NODIFF};

struct fileEntry{

  char* path;
  char* URL;
  char* date;
  char* size;
  unsigned short dateI;

  /*Porque cuando 'differ' devuelve entradas,
    debe almacenar que tipo de diferencia era*/
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
//     - netInfo: almacena la dirección IP que corresponde al dominio
//     - next: próximo elemento de la lista URL
struct URL typedef URL;

struct URL
{
  char *dir;
  char *domain;
  struct addrinfo *netInfo;
  struct URL *next;
};

// La estructura workerInfo se utiliza para solucionar 
// la limitante que tienen las funciones threads en C 
// las cuales se les pueden pasar un único argumento.
struct workerInfo typedef workerInfo;

struct workerInfo 
{
  unsigned int time;
  struct URL* urlList;
};

#endif
