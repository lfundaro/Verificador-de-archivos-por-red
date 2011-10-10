#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

enum diff_type{NEW , CHANGE , NODIFF};

struct entry{

  char* path;
  char* URL;
  char* date;
  char* size;
  unsigned short dateI;

  /*Porque cuando 'differ' devuelve entradas,
    debe almacenar que tipo de diferencia era*/
  enum diff_type dt;

} typedef entry;

struct entry_node typedef entry_node;
struct entry_node{
  entry e;
  entry_node* next;
};

struct URL typedef URL;
struct URL
{
  char dir[2048];
  char domain[1024];
  struct URL *next;
};

#endif
