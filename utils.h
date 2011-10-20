#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifndef UTILS_H
#define UTILS_H

/** \file */

/**

   Extrae las líneas especificadas en el archivo 
   y almacena la información en la estructura URL. 
   
   \brief Extrae información de archivos.
   
   \param FILE* File descriptor.

   \return No retorna valor.
*/
URL *
parseFile (FILE* fd);

/**

   Concatena una barra al final del directorio 
   si éste no se encuentra presente.
   
   \brief Completar con barra.
   
   \param dir. Directorio a verificar.

   \return Retorna 1 si agregó una barra al final 
   del parámetro dir; 0 si no.
*/
int
slash_append (char *dir);

/**

   Verifica si el directorio especificado por el usuario 
   con la opción -d existe dentro del archivo especificado 
   por la opción -a
   
   \brief Verifica consistencia entre opción -d y contenido 
   de archivo.
   
   \param dir. Directorio especificado por el usuario.

   \param urlList. Lista enlazada de estructuras URL.

   \return Retorna 0 si valor de dir corresponde con contenido
   en archivo; -1 si el valor de dir no corresponde.
*/
int
file_lookup (char * dir, URL *urlList);

void
init_entry(fileEntry* e);

/**
   Imprime los posibles mensajes de error que la librería 'regex.h'
   puede generar.

   \brief Imprime mensajes de error de 'regex.h'

   \param errcode Código del error.
*/
void handle_regex_errors(int errcode);

/**
   Llama a malloc, en caso de que malloc falle llama a exit().

   \brief Llama a malloc de forma segura.

   \param size Cantidad de memoria (en bytes) a reservar.
*/
void* smalloc(int size);

/**
   Traduce los primeros 'num_length' carácteres del arreglo 'asciinum' a un
   número en base decimal. El número en la posición cero se considera el menos
   significativo.

   \brief Transforma un número decimal en ASCII a complemento dos.

   \param asciinum Arreglo con el número en ASCII.

   \param num_length Cantidad de dígitos a traducir del arreglo 'asciinum'.

   \return Número transformado.
*/
int asciinum_to_int(char* asciinum, int num_length);



/**
   Traduce el carácter 'c' a un número del 0 al 9. Si el carácter
   'c' no tiene un valor ASCII entre 48 y 57 se retorna -1.

   \brief Transforma un carácter decimal en ASCII a complemento dos.

   \param c Carácter con el número en ASCII.

   \return Número transformado.
*/
int asciichar_to_int(char c);



/**
   Imprime mensajes pertinentes para los diferentes códigos definidos
   para el protocolo HTTP.

   \brief Imprime mensajes para los diferentes códigos HTTP.

   \param http_code Código del mensaje HTTP.

   \param url %URL que se le concatenará al mensaje.

   \return Retorna 1 (error) si el código HTTP era de redirección (3xx), 
           error del cliente (4xx) o error del servidor (5xx).
	   Para los otros tipos de código (1xx y 2xx) retorna 0 (éxito)
*/
int http_error_handler(char* http_code, char* url);

/**
   Hace una copia del apuntador de la estructura ENTRY 
   que se almacena en la tabla de hash para después 
   liberar dicho apuntador con todas sus subestructuras 
   
   \brief    Agrega apuntador ENTRY a la lista de control.
   
   \param p. Tupla (key, valor).

   \param  controlNodes. Estructura que mantiene 
   control de otras estructuras en memoria.

   \return No retorna valor.
*/
void 
add_eControl (ENTRY *p, eControl **controlNodes);

/**
   
   Función que libera los datos guardados dentro de la 
   tabla de hash.
   
   \brief Agrega apuntador ENTRY a la lista de control.
   
   \param controlNodes. Estructura que mantiene 
   control de otras estructuras en memoria.

   \return No retorna valor.
*/
void 
free_ENTRY (eControl **controlNodes);
#endif
