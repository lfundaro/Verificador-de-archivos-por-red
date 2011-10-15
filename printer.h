#ifndef PRINTER_H
#define PRINTER_H

#include "data_structures.h"

/** \file */


/**
   Imprime los datos del fileEntry 'fe', usando el campo 'dt' para determinar
   si es una entrada cambiada o nueva.

   \brief Imprime los datos del fileEntry 'fe'.

   \param fe Entrada de archivo a imprimir.
*/
void print(struct fileEntry fe);



/**
   Imprime los datos de los fileEntry en la lista 'fel', usando el campo 'dt'
   para determinar si es una entrada cambiada o nueva.

   \brief Imprime los datos de los fileEntry en la lista 'fel'.

   \param fel Lista de fileEntry a imprimir.
*/
void printer(entry_node* fel);

#endif
