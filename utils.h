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

URL *
parseFile (FILE*);

int
file_lookup (char *, URL *);

void
init_entry(fileEntry* e);

//Manejar errores de la libreria 'regex.h'
void handle_regex_errors(int errcode);

int asciinum_to_int(char c);

//Manejar los codigo de error de HTTP
void http_error_handler(char* http_code, char* url);
#endif
