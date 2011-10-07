#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#ifndef UTILS_H
#define UTILS_H

struct URL
{
  char dir[2048];
  char domain[1024];
  struct URL *next;
} typedef URL;

URL *
parseFile (FILE*);

int
file_lookup (char *, URL *);

#endif
