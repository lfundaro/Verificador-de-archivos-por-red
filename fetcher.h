#ifndef FETCHER_H
#define FETCHER_H

#include <netdb.h>

//For retrieving the HTML text of ONE directory
int fetch(struct addrinfo,char**);

//For retrieving the HTML text of the target directories
char** fetcher(struct addrinfo*,unsigned short);

#endif
