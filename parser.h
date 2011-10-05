#ifndef PARSER_H
#define PARSER_H

#include "data_structures.h"

//Funcion que parsea una pagina
int parse(char*,struct entry_node**);

//Funcion que parsea cada pagina descargada
entry_node* parser(char**,unsigned short);

#endif
