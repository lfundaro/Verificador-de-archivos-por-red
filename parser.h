#ifndef PARSER_H
#define PARSER_H

#include <regex.h>
#include <string.h>
#include <stdio.h>

#include "globals.h"
#include "data_structures.h"
#include "utils.h"

/** \file */


/**
   Parsea cada página en el arreglo 'pgs' y genera una lista de entry, cada fileEntry representa un archivo.

  \brief Parsea cada página en 'pgs'.

  \param pgs Arreglo de páginas a parsear.

  \param npgs Número de páginas en el arreglo 'pgs'.

  \return Lista de fileEntry que contendrá las entradas de archivo detectadas en las páginas parseadas.
*/
entry_node* parser(char** pgs, unsigned short npgs);



/**
   Parsea la página apuntada por 'pg'. Genera una lista de fileEntry para la página parseada.

   \brief Parsea la página apuntada por 'pg'.

   \param pg Apuntador a la página a parsear.

   \param list Lista de fileEntry donde se guardarán las entradas de archivos detectadas en la página parseada.
   
   \return Código de terminación. Cero significa exitoso.
*/
int parse(char* pg, struct entry_node** list);



/**
   Las páginas descargadas tienen el %URL de la página que se descargó concatenada al principio del contenido. Esta función recupera esa %URL para que pueda se usar asociar con las entradas de archivo encontradas en la página parseada. El HTML donde se buscará la entrada está apuntado por 'pg', dominio se guardará en (*base_url) y el path dentro del dominio se guardará en (*path).

   \brief Recupera una %URL concatenada al principio del HTML apuntado por 'pg'.

   \param pg Apuntador al HTML donde se buscará el %URL concatenado.

   \param path String donde se guardará el path del %URL concatenado.

   \param base_url String donde se guardará el dominio del %URL concatenado.

   \return Se asume que el %URL siempre se encuentra al principio de una página descargada, por lo que no se devuelve código de terminación.
 */
void parse_url(char* pg, char** path, char** base_url);



/**
   Hace match con la primera entrada de archivo en el HTML. Guarda la información sobre el match en el arreglo '(*matches_ptr)'.
   
   \brief Hace match con la primera entrada de archivo en el HTML.

   \param pg Apuntador al HTML donde se tratará de detectar la primera entrada.

   \param matches_ptr Arreglo que contendrá el desplazamiento en el HTML donde se encuentra el match.

   \param cpattern_ptr Expresión regular compilada que se usa para hacer match de las entradas de archivos.

   \return Código de terminación. Cero significa exitoso.
*/
int match_entry(char* pg, regmatch_t** matches_ptr, regex_t** cpattern_ptr);



/**
   Construye un fileEntry a partir de una entrada de archivo en el HTML. La información sobre la localización de la entrada en el HTML se encuentra en 'matches_ptr'. Para construir el fileEntry también se usa el path y el %URL de la página que se esta parseando, esta información se encuentra en los string apuntados por 'path' y 'base_url' respectivamente.

   \brief Construye un fileEntry a partir de una entrada de archivo en el HTML.

   \param pg Apuntador al HTML donde se encuentra la entrada de archivo a recuperar.

   \param base_url El dominio de la página de la que se descargó el contenido.

   \param path Path dentro del dominio del que se descargó el contenido.

   \param matches_ptr Arreglo con los desplazamientos de la entrada de archivo dentro del HTML.

   \return Nuevo fileEntry con la información de la entrada encontrada.
*/
fileEntry parse_entry(char* pg, char* base_url, char* path,
		      regmatch_t** matches_ptr);

/**
  Recupera el string de una subexpresión (encerrada por '\(' y '\)' dentro de la expresion regular) de un match de la expresion regular. La subexpresión a recuperar esta especificada por data_pos, y representa un índice dentro del arreglo matches_ptr. La página donde se hizo el match esta apuntada por 'pg'.

   \brief Recupera el string de una subexpresión de un match de la expresion regular.

   \param data_pos Índice dentro del arreglo (*matches_ptr) donde se encuentra la información de la subexpresión buscada.

   \param pg HTML donde se encuentra la subexpresión.

   \param data_pos Arreglo donde se encuentra la información sobre los desplazamientos de las subexpresiones dentro del HTML.
*/
char* get_data(int data_pos,char* pg,regmatch_t** matches_ptr);
#endif
