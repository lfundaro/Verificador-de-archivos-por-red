#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

enum diff_type{ NEW , CHANGE , NODIFF};

struct entry{

  char* name;
  char* URL;
  unsigned short date;

  /*Porque cuando 'differ' devuelve entradas,
    debe almacenar que tipo de diferencia era*/
  enum diff_type dt;

} typedef entry;

struct entry_node typedef entry_node;

struct entry_node{
  entry e;
  entry_node* next;
};
#endif
