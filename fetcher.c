#include "fetcher.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Funcion que descarga las paginas HTTP que se encuentran en el arreglo 'addrs'
char**
fetcher(URL* url_list, int* nurls){
  //variables de iteracion
  URL* li = NULL;
  int i = 0;

  //Numero de paginas a descargar
  (*nurls) = 0;
  
  //Contar el numero de paginas
  for(li = url_list; li != NULL; li = li->next){
    (*nurls)++;
  }

  //reservar espacio para apuntadores a las paginas
  char** pgs = (char**)smalloc(sizeof(char*)*(*nurls)); 
  
  //iterar por las direcciones
  for(li = url_list; li != NULL; li = li->next){
    //descargar pagina, se guarda en 'pgs'
    int retc = fetch(li,&(pgs[i]));
    //    printf("fetched webpage %d\n",i); // (FLAG)

    if (retc){
      exit(EXIT_FAILURE);//salir si hubo un error
    }
    ++i;
  }

  return pgs;
}

//Funcion que descarga una pagina HTTP
//en el bufer '*(pg_ptr)' desde la direccion 'pg_addr'
int
fetch(URL* url, char** pg_ptr){
  int sock_des = 0;//descriptor de archivo que retorna socket()
  int ret = 0; //para guardar valores de retorno

  //Cabecera que sera a~nadida al tope del HTML descargado
  //para que el parser sepa el URL de las entradas
  int url_header_sz = strlen(url->dir) + 1;
  char* url_header = (char*)smalloc(sizeof(char)*(url_header_sz));
  strcpy(url_header,url->dir);
  url_header[url_header_sz-1] = '\0';

  //Direccion de la pagina
  struct addrinfo pg_addr = *(url->netInfo);

  //Crear socket. IPv4/TCP
  sock_des = socket(pg_addr.ai_family,
		    pg_addr.ai_socktype,
		    pg_addr.ai_protocol);
  if (sock_des == -1){
    perror("Error creando el socket: ");
    exit(EXIT_FAILURE);
  }

  //Conectar a la pagina usando el socket
  ret = connect(sock_des,
		pg_addr.ai_addr, 
		pg_addr.ai_addrlen);
  if(ret){
    perror("Error conectandose: ");
    exit(EXIT_FAILURE);
  }

  //Construir instruccion HTTP.
  const char* aux_get_first = "GET ";
  const char* aux_get_second = " HTTP/1.0\r\n\r\n";
  int url_len =  strlen(url->dir);
  int aux_get_len = strlen(aux_get_first) + strlen(aux_get_second);

  char* http_get = (char*)smalloc(sizeof(char)*(url_len + aux_get_len +1));
  strcpy(http_get,aux_get_first);
  strcat(http_get,url->dir);
  strcat(http_get,aux_get_second);
  int get_len = strlen(http_get);

  //  printf("%s\n",http_get);//(FLAG)

  //Enviar paquete HTTP
  ret = write(sock_des, (void*)http_get, get_len);
  if(ret < 0){
    perror("Error escribiendo en el socket: ");
    exit(EXIT_FAILURE);
  }
  if(ret < get_len){
    perror("Error, transmision incompleta/fallida: ");
    exit(EXIT_FAILURE);
  }

  //Descargar pagina
  download_page(pg_ptr,sock_des,url_header,url_header_sz);

  //Recuperar codigo HTTP y manejarlo
  char http_code[3];
  http_code[0] = (*pg_ptr)[url_header_sz+1 + 8];
  http_code[1] = (*pg_ptr)[url_header_sz+2 + 8];
  http_code[2] = (*pg_ptr)[url_header_sz+3 + 8];
  http_error_handler(http_code, url_header);

  //(FLAG) Imprimir pagina descargada
  //  printf("%s\n",*(pg_ptr));

  free(url_header);
  free(http_get);

  //Retornar exito
  return 0;
}

//Descarga una pagina HTTP en '*(pg_ptr)' usando el socket
//apuntado por 'sock_des'
char* download_page(char** pg_ptr, int sock_des,
		    char* url_header, int url_header_sz){
  /**Lee pagina HTTP en bloques de tama~no 'blck_sz'**/

  //Tama~no de bloque
  int blck_sz = 128;

  //Cantidad de bytes por leer del bloque actual
  int pending_sz = blck_sz;

  //Tama~no maximo del bufer. Crece con el numero de bloques que se agregan
  int max_buff_sz = blck_sz;

  //Tama~no actual del bufer. Cantidad de bytes en uso.
  int total_buff_sz = 0;

  //Reserva inicial de espacio para la pagina
  *(pg_ptr) = (char*)smalloc(sizeof(char)*(url_header_sz + blck_sz + 1));//'+ 1' por el byte nulo

  //Apuntador donde escribira read()
  char* write_ptr = *(pg_ptr);
  
  //bytes leidos en una llamada a read()
  ssize_t read_bytes = 0;

  //A~nadir la cabecera para que el
  //parser sepa el URL de las entradas
  strcpy((*pg_ptr),url_header);
  (*pg_ptr)[url_header_sz-1] = '\n';
  write_ptr += url_header_sz;

  //Mientras read() lea algo
  while((read_bytes = read(sock_des,write_ptr,pending_sz)) > 0){

    write_ptr += read_bytes;
    pending_sz -= read_bytes;
    total_buff_sz += read_bytes;

    //Si el tama~no del buffer alcanzo el maximo
    if(total_buff_sz == max_buff_sz){

      //Crear un nuevo bufer ('+ 1' por el byte nulo)
      char* t = (char*)smalloc(sizeof(char)*(total_buff_sz + blck_sz + 1));
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

  if(read_bytes < 0){
    perror("Error leyendo del socket: ");
    exit(EXIT_FAILURE);
  }

  *(write_ptr) = '\0';//Byte nulo de terminacion

  return *(pg_ptr);
}
