#ifndef FETCHER_H
#define FETCHER_H

#include <netdb.h>

#include "data_structures.h"

/** \file */


/**
 Descarga las paginas HTTP que se encuentran en la lista 'url_list'.
 Modifica el valor de 'nurls' para contener el número de elementos
 en la lista 'url_list'

 \brief Descarga una lista de páginas HTTP.

 \param url_list Lista de URL que se van a descargar.

 \param nurls Parámetro de salida por referencia que contendrá el
              numero de URLs que hay en la lista 'url_list'.

 \return Arreglo con las páginas descargadas
 */
char** fetcher(URL* url_list, int* nurls);



/**
  Descarga una página HTTP en el bufer '*(pg_ptr)' desde la direccion
  almacenada en url->netInfo. Esta función abre la conección y construye
  la petición HTTP GET, llama a una función auxiliar para hacer la descarga.

  \brief Envía una petición HTTP GET y guarda el contenido de la respuesta.

  \param url URL con la informacion necesaria para generar la petición HTTP.

  \param pg_ptr Apuntador donde se guardara la página descargada.

  \return Código de terminación. Cero significa exitoso.
*/
int fetch(URL* url,char** pg_ptr);



/**
   Descarga una página HTTP en *(pg_ptr) usando el socket
   apuntado por 'sock_des'. Esta función también concatena
   el %URL que fue solicitado al principio del contenido
   descargado. También maneja los posibles errores HTTP.

   \brief Descarga una página HTTP usando el socket
          apuntado por 'sock_des'.

   \param pg_ptr Buffer donde se guardará el contenido.

   \param sock_des Socket desde donde se leerá la página.

   \param url_header %URL que fue solicitado y que será concatenado
                     al principio del contenido a descargar.

   \param url_header_sz Tamaño en bytes del %URL a concatenar.

   \return Código de terminación. Cero significa exitoso.
*/
int download_page(char** pg_ptr, int sock_des,
		  char* url_header, int url_header_sz);



/**
   Descarga el header de una respuesta HTTP, usando el socket apuntado por
   'sock_des', en un buffer y devuelve el apuntador del buffer.

   \brief Descarga el header de una respuesta HTTP.

   \param sock_des Socket desde donde se leerá el header.

   \return Apuntador al buffer donde se guardará el header.
*/
char* download_header(int sock_des);



/**
   Descarga el contenido de una respuesta HTTP, usando el socket
   apuntado por 'sock_des', en un buffer de tamaño clength y 
   devuelve el apuntador del buffer.

   \brief Descarga el contenido de una respuesta HTTP.

   \param sock_des Socket desde donde se leerá el header.

   \param content String en donde se descargará el contenido.

   \param clength Tamaño en bytes del contenido a descargar.

   \param url_header %URL que fue solicitado y que será concatenado
                     al principio del contenido a descargar.

   \param url_header_sz Tamaño en bytes del %URL que será concatenado.

   \return Código de error. Cero significa exitoso.
*/
int download_body_clength(int sock_des, char** content,
			  int clength, char* url_header,
			  int url_header_sz);



/**
   Descarga el contenido de una respuesta HTTP de tamaño desconocido,
   usando el socket apuntado por 'sock_des', en un buffer y devuelve
   el apuntador del buffer.

   \brief Descarga el contenido de una respuesta HTTP.

   \param sock_des Socket desde donde se leerá el header.

   \param content String en donde se descargará el contenido.

   \param blck_sz La función descargará el contenido en bloques
   de tamaño 'blk_sz' bytes hasta que no quede contenido en el
   soket.

   \param url_header %URL que fue solicitado y que será concatenado
                     al principio del contenido a descargar.

   \param url_header_sz Tamaño en bytes del %URL que será concatenado.

   \return Código de error. Cero significa exitoso.
*/
int download_body_no_clength(int sock_des, char** content, 
			     int blck_sz, char* url_header,
			     int url_header_sz);
#endif
