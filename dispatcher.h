#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "data_structures.h"
#include "fetcher.h"
#include "parser.h"
#include "differ.h"
#include "printer.h"

//La tabla de hash es global.
void 
dispatcher(URL* url_list, eControl **controlNodes);

#endif
