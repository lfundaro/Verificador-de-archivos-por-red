#include "fetcher.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Funcion que descarga una pagina por
//http en la direccion pg_addr en el bufer *(pg_ptr) 
int fetch(struct addrinfo pg_addr,char** pg_ptr){
  int sock_des = 0;//file descriptor returned by socket()
  int ret = 0; //to hold return values

  //Crear socket. IPv4/TCP
  sock_des = socket(pg_addr.ai_family,
		    pg_addr.ai_socktype,
		    pg_addr.ai_protocol);
  if (sock_des == -1){
    printf("Error creating socket");
    exit(1);
  }


  //Conectar a la pagina usando el socket
  ret = connect(sock_des,
		pg_addr.ai_addr, 
		pg_addr.ai_addrlen);
  if(ret){
    printf("Error connecting");
    exit(1);
  }

  //Construir instruccion HTTP.
  const char* http_get = "GET /~german/ HTTP/1.0\r\n\r\n";
  int getlen = strlen(http_get);

  //Enviar paquete HTTP
  ret = write(sock_des, (void*)http_get, getlen);
  if(ret < 0){
    printf("Error writing to socket");
    exit(1);
  }
  if(ret < getlen){
    printf("Error incomplete/failed transmition");
    exit(1);
  }
  
  /**Leer respuesta en bloques de tama~no 'blck_sz'**/

  //Tama~no de bloque
  int blck_sz = 128;

  //Cantidad de bytes por leer del bloque actual
  int pending_sz = blck_sz;

  //Tama~no maximo del bufer. Crece con el numero de bloques que se agregan
  int max_buff_sz = blck_sz;

  //Tama~no actual del bufer. Cantidad de bytes en uso.
  int total_buff_sz = 0;

  //Reserva inicial de espacio para la pagina
  *(pg_ptr) = (char*)malloc(sizeof(char)*(blck_sz + 1));//'+ 1' por el byte nulo

  //Apuntador donde escribira read()
  char* write_ptr = *(pg_ptr);
  
  //bytes leidos en una llamada a read()
  ssize_t read_bytes = 0;

  //Mientras read() lea algo
  while((read_bytes = read(sock_des,write_ptr,pending_sz)) > 0){

    write_ptr += read_bytes;
    pending_sz -= read_bytes;
    total_buff_sz += read_bytes;

    //Si el tama~no del buffer alcanzo el maximo
    if(total_buff_sz == max_buff_sz){

      //Crear un nuevo bufer ('+ 1' por el byte nulo)
      char* t = (char*)malloc(sizeof(char)*(total_buff_sz + blck_sz + 1));
      memcpy(t,*(pg_ptr),total_buff_sz);

      //Actualizar apuntadores
      free((*pg_ptr));
      *(pg_ptr) = t;
      write_ptr = *(pg_ptr)+total_buff_sz;

      //Actualizar la cantidad de bytes por leer y el tama~no maximo
      pending_sz = blck_sz;
      max_buff_sz += blck_sz;
    }
  }

  *(write_ptr) = '\0';//Byte nulo de terminacion

  if(read_bytes < 0){
    printf("Error reading socket");
    exit(1);
  }

  //(FLAG) Imprimir pagina descargada
  printf("%s\n",*(pg_ptr));

  //Retornar exito
  return 0;
}

char** fetcher(struct addrinfo* addrs, unsigned short naddrs){
  int i; //variable de iteracion

  //reservar espacio para apuntadores a las paginas
  char** pgs = (char**)malloc(sizeof(char*)*naddrs); 
  
  //iterar por las direcciones
  for(i = 0; i < naddrs; ++i){

    //descargar pagina - se guarda en 'pgs'
    int retc = fetch(addrs[i],&(pgs[i]));
    printf("fetched entry %d\n",i);

    if (retc){
      exit(1);//salir si hubo un error
    }
  }

  return pgs;
}
