#include "resolver.h"

// Resuelve direcciones URL a IP.
// Almacena las direcciones IP en la estructura
// netInfo.
void
resolve (URL *urlList)
{
  struct URL *it = urlList;
  int err;
  
  while (it != NULL)
    {
      // resolver dominio a ip
      err = getaddrinfo (it->domain, "http", NULL, &(it->netInfo));
      if (err != 0)
        fprintf (stderr, "error resolviendo dirección: %s\n", gai_strerror(err));
      it = it->next;
    }
  return;
}
