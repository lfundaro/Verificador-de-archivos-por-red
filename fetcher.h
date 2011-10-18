#ifndef FETCHER_H
#define FETCHER_H

#include <netdb.h>

#include "data_structures.h"

/** \file */


/**
 Descarga las paginas HTTP que se encuentran en la lista 'url_list'.
 Modifica el valor de 'nurls' para contener el n�mero de elementos
 en la lista 'url_list'

 \brief Descarga una lista de p�ginas HTTP.

 \param url_list Lista de URL que se van a descargar.

 \param nurls Par�metro de salida por referencia que contendr� el
              numero de URLs que hay en la lista 'url_list'.

 \return Arreglo con las p�ginas descargadas
 */
char** fetcher(URL* url_list, int* nurls);



/**
  Descarga una p�gina HTTP en el bufer '*(pg_ptr)' desde la direccion
  almacenada en url->netInfo. Esta funci�n abre la conecci�n y construye
  la petici�n HTTP GET, llama a una funci�n auxiliar para hacer la descarga.

  \brief Env�a una petici�n HTTP GET y guarda el contenido de la respuesta.

  \param url URL con la informacion necesaria para generar la petici�n HTTP.

  \param pg_ptr Apuntador donde se guardara la p�gina descargada.

  \return C�digo de terminaci�n. Cero significa exitoso.
*/
int fetch(URL* url,char** pg_ptr);



/**
   Descarga una p�gina HTTP en *(pg_ptr) usando el socket
   apuntado por 'sock_des'. Esta funci�n tambi�n concatena
   el %URL que fue solicitado al principio del contenido
   descargado. Tambi�n maneja los posibles errores HTTP.

   \brief Descarga una p�gina HTTP usando el socket
          apuntado por 'sock_des'.

   \param pg_ptr Buffer donde se guardar� el contenido.

   \param sock_des Socket desde donde se leer� la p�gina.

   \param url_header %URL que fue solicitado y que ser� concatenado
                     al principio del contenido a descargar.

   \param url_header_sz Tama�o en bytes del %URL a concatenar.

   \return C�digo de terminaci�n. Cero significa exitoso.
*/
int download_page(char** pg_ptr, int sock_des,
		  char* url_header, int url_header_sz);



/**
   Descarga el header de una respuesta HTTP, usando el socket apuntado por
   'sock_des', en un buffer y devuelve el apuntador del buffer.

   \brief Descarga el header de una respuesta HTTP.

   \param sock_des Socket desde donde se leer� el header.

   \return Apuntador al buffer donde se guardar� el header.
*/
char* download_header(int sock_des);



/**
   Descarga el contenido de una respuesta HTTP, usando el socket
   apuntado por 'sock_des', en un buffer de tama�o clength y 
   devuelve el apuntador del buffer.

   \brief Descarga el contenido de una respuesta HTTP.

   \param sock_des Socket desde donde se leer� el header.

   \param content String en donde se descargar� el contenido.

   \param clength Tama�o en bytes del contenido a descargar.

   \param url_header %URL que fue solicitado y que ser� concatenado
                     al principio del contenido a descargar.

   \param url_header_sz Tama�o en bytes del %URL que ser� concatenado.

   \return C�digo de error. Cero significa exitoso.
*/
int download_body_clength(int sock_des, char** content,
			  int clength, char* url_header,
			  int url_header_sz);



/**
   Descarga el contenido de una respuesta HTTP de tama�o desconocido,
   usando el socket apuntado por 'sock_des', en un buffer y devuelve
   el apuntador del buffer.

   \brief Descarga el contenido de una respuesta HTTP.

   \param sock_des Socket desde donde se leer� el header.

   \param content String en donde se descargar� el contenido.

   \param blck_sz La funci�n descargar� el contenido en bloques
   de tama�o 'blk_sz' bytes hasta que no quede contenido en el
   soket.

   \param url_header %URL que fue solicitado y que ser� concatenado
                     al principio del contenido a descargar.

   \param url_header_sz Tama�o en bytes del %URL que ser� concatenado.

   \return C�digo de error. Cero significa exitoso.
*/
int download_body_no_clength(int sock_des, char** content, 
			     int blck_sz, char* url_header,
			     int url_header_sz);
#endif
