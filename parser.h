#ifndef PARSER_H
#define PARSER_H

#include <regex.h>
#include <string.h>
#include <stdio.h>

#include "globals.h"
#include "data_structures.h"
#include "utils.h"

//Funcion que parsea cada pagina descargada
entry_node* parser(char**,unsigned short);

//Funcion que parsea una pagina
int parse(char*,struct entry_node**);

//Funcion que hace match con la ultima entrada del HTML
int match_entry(char* pg,
		regmatch_t** matches_ptr,
		regex_t** cpattern_ptr);

void parse_url(char* pg, char** path, char** base_url);

//Funcion que construye un 'struct entry' a partir de una entrada en el HTML
fileEntry parse_entry(char* pg, char* base_url, char* path,
		  regmatch_t** matches_ptr);

//Funcion que recupera un subcampo de un match de la expresion regular
char* get_data(int data_pos,char* pg,regmatch_t** matches_ptr);

//Manejar errores de la libreria 'regex.h'
void handle_regex_errors(int errcode);

#endif
