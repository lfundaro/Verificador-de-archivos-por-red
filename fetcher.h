#ifndef FETCHER_H
#define FETCHER_H

#include <netdb.h>

//Funcion que descarga las paginas HTTP que se encuentran en el arreglo 'addrs'
char** fetcher(struct addrinfo* addrs,unsigned short naddrs);

//Funcion que descarga una pagina HTTP
//en el bufer '*(pg_ptr)' desde la direccion 'pg_addr'
int fetch(struct addrinfo pg_addr,char** pg_ptr);

//Descarga una pagina HTTP en *(pg_ptr) usando el socket
//apuntado por 'sock_des'
char* download_page(char** pg_ptr, int sock_des);
#endif
