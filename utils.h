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
parseFile (FILE*);
/**

   Verifica si el directorio especificado por el usuario 
   con la opción -d existe dentro del archivo especificado 
   por la opción -a
   
   \brief Verifica consistencia entre opción -d y contenido 
   de archivo.
   
   \param char* dir. Directorio especificado por el usuario.

   \param URL* urlList. Lista enlazada de estructuras URL.

   \return Retorna 0 si valor de dir corresponde con contenido
   en archivo; -1 si el valor de dir no corresponde.
*/
int
file_lookup (char *, URL *);

void
init_entry(fileEntry* e);

//Manejar errores de la libreria 'regex.h'
void handle_regex_errors(int errcode);

void* smalloc(int size);

int asciinum_to_int(char* asciinum, int num_length);

int asciichar_to_int(char c);

//Manejar los codigo de error de HTTP

int http_error_handler(char* http_code, char* url);

/**
   Hace una copia del apuntador de la estructura ENTRY 
   que se almacena en la tabla de hash para después 
   liberar dicho apuntador con todas sus subestructuras 
   
   \brief    Agrega apuntador ENTRY a la lista de control.
   
   \param ENTRY *p. Tupla (key, valor).

   \param eControl **controlNodes. Estructura que mantiene 
   control de otras estructuras en memoria.

   \return No retorna valor.
*/
void 
add_eControl (ENTRY *p, eControl **controlNodes);

/**
   
   Función que libera los datos guardados dentro de la 
   tabla de hash.
   
   \brief Agrega apuntador ENTRY a la lista de control.
   
   \param eControl **controlNodes. Estructura que mantiene 
   control de otras estructuras en memoria.

   \return No retorna valor.
*/
void 
free_ENTRY (eControl **controlNodes);
#endif
