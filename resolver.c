#include "resolver.h"

void
resolve (URL *urlList)
{
  struct URL *it = urlList;
  int err;
  
  while (it != NULL)
    {
      // resolver dominio a ip
      err = getaddrinfo (it->domain, "80", NULL, &(it->netInfo));
      if (err != 0)
        fprintf (stderr, "error resolviendo dirección: %s\n", gai_strerror(err));
      it = it->next;
    }
  return;
}
