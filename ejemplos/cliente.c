/* Programa cliente */
/* Ejemplo tomado del libro: Linux Application Development. Michael 
   K. Johnson y Erik Troan */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
 
#include "sockutil.h"

/* Se conecta al servidor cuyo hostname o IP recibe como argumento por el puerto 1236. Una vez que se conecta copia todo lo que se introduce por stdin al socket. */

int main(int argc, char ** argv)
{

    struct sockaddr_in address;
    struct in_addr inaddr;
    struct hostent *host;
    int sock;


    if (argc != 2) {
      fprintf(stderr, "only a single argument is supported\n");
      return 1;
    }

    /* Si el argumento se puede convertir en una direccion IP lo hace, de lo contrario busca en DNS */
    if (inet_aton(argv[1], &inaddr))
      host = gethostbyaddr((char *) &inaddr, sizeof(inaddr), AF_INET);
    else
      host = gethostbyname(argv[1]);

    if (!host) {
      herror("error looking up host");
      exit(1);
    }

    if(( sock = socket (PF_INET, SOCK_STREAM, 0)) < 0)
		die("socket");
    
    address.sin_family = AF_INET;
    address.sin_port = htons(1236);
    /* Toma la primera direcci'on IP asociada a este hostname */
    memcpy(&address.sin_addr, host->h_addr_list[0], sizeof(address.sin_addr));

    if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
		die("connect");

    copyData(0, sock);

    close(sock);
    return 0;

}
