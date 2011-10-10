#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

enum diff_type{ NEW , CHANGE , NODIFF};

struct fileEntry{

  char* name;
  char* URL;
  char* date;

  /*Porque cuando 'differ' devuelve entradas,
    debe almacenar que tipo de diferencia era*/
  enum diff_type dt;

} typedef fileEntry;

struct entry_node typedef entry_node;
struct entry_node{
  fileEntry e;
  entry_node* next;
};

struct URL typedef URL;

struct URL
{
  char *dir;
  char *domain;
  struct addrinfo *netInfo;
  struct URL *next;
};

struct workerInfo typedef workerInfo;

struct workerInfo 
{
  unsigned int time;
  struct URL* urlList;
};

#endif
