#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#ifndef UTILS_H
#define UTILS_H

URL *
parseFile (FILE*);

int
file_lookup (char *, URL *);

#endif
