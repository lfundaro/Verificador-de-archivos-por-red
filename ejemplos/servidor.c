/* Programa Servidor. ejemplo con sockets TCP/IP */
/* Ejemplo tomado del libro: Linux Application Development. Michael 
   K. Johnson y Erik Troan */

   

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
 
#include "sockutil.h"

/* Espera por una conexi'on por el puerto 1236. Una vez que la conexion se 
   establece, se copian los datos enviados por el socket a la salida estandar y
   se espera por otra conexion */

int main(void)
{
	struct sockaddr_in address;
        int sock, conn, i;
	size_t addrLength = sizeof(struct sockaddr_in);


        if(( sock = socket (PF_INET, SOCK_STREAM, 0)) < 0)
		die("socket");


	/* Permite reusar la direccion del socket en un periodo corto de tiempo */ 
       i = 1;
       setsockopt(sock,SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
       

       address.sin_family = AF_INET;
       address.sin_port = htons(1236);
       memset(&address.sin_addr, 0, sizeof(address.sin_addr));

       if (bind(sock, (struct sockaddr *)&address, sizeof(address)))
		die("bind");

       if (listen(sock, 5))
		die("listen");
       while((conn = accept(sock, (struct sockaddr *) &address, &addrLength)) >= 0) {
		printf("...getting data\n");
                copyData(conn,1);
                printf("...Done\n");
                close(conn);
     }

     if (conn <  0) die("accept");
     close(sock);
     return 0;


}

