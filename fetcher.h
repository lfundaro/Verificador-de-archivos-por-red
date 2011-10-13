#ifndef FETCHER_H
#define FETCHER_H

#include <netdb.h>

#include "data_structures.h"

//Funcion que descarga las paginas HTTP que se encuentran en el arreglo 'addrs'
char** fetcher(URL* url_list, int* nurls);

//Funcion que descarga una pagina HTTP
//en el bufer '*(pg_ptr)' desde la direccion 'pg_addr'
int fetch(URL* url,char** pg_ptr);

//Descarga una pagina HTTP en *(pg_ptr) usando el socket
//apuntado por 'sock_des'
int download_page(char** pg_ptr, int sock_des,
		  char* url_header, int url_header_sz);

//Descarga el header de una respuesta HTTP
//usando el socket apuntado por 'sock_des'
char* download_header(int sock_des);
#endif
