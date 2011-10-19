#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "fetcher.h"
#include "regex.h"

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

    if (retc){
      (pgs[i]) = NULL;
    }
    ++i;
  }

  return pgs;
}

int
fetch(URL* url, char** pg_ptr){
  int sock_des = 0;//descriptor de archivo que retorna socket()
  int ret = 0; //para guardar valores de retorno

  //Cabecera que sera añadida al tope del HTML descargado
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
    perror("Error creando el socket");
    exit(EXIT_FAILURE);
  }

  //Conectar a la pagina usando el socket
  ret = connect(sock_des,
		pg_addr.ai_addr, 
		pg_addr.ai_addrlen);
  if(ret){
    perror("Error conectandose");
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

  //Enviar paquete HTTP
  ret = write(sock_des, (void*)http_get, get_len);
  if(ret < 0){
    perror("Error escribiendo en el socket");
    free(url_header);
    free(http_get);
    exit(EXIT_FAILURE);
  }
  if(ret < get_len){
    perror("Error, transmision incompleta/fallida");
    free(url_header);
    free(http_get);
    exit(EXIT_FAILURE);
  }

  //Descargar pagina
  if (download_page(pg_ptr,sock_des,url_header,url_header_sz)){
    free(url_header);
    free(http_get);
    return 1;
  }

  //  printf ("%s\n", (*pg_ptr)); // FLAG

  free(url_header);
  free(http_get);

  //Retornar exito
  return 0;
}

int
download_page(char** pg_ptr, int sock_des,
	      char* url_header, int url_header_sz){
  int ret; //Para guardar valores de retorno

  char* http_header = download_header(sock_des);
  printf ("HEADER: %s\n", (http_header)); // FLAG

  //Recuperar código HTTP y manejarlo
  char http_code[3];
  http_code[0] = http_header[8 + 1];
  http_code[1] = http_header[8 + 2];
  http_code[2] = http_header[8 + 3];
  if (http_error_handler(http_code, url_header)){
    free(http_header);
    return 1;
  }

  /*Recuperar el tamaño de contenido de la respuesta HTTP*/
  regmatch_t* matches = (regmatch_t*)smalloc(sizeof(regmatch_t)*2);
  regex_t* cpattern = (regex_t*)smalloc(sizeof(regex_t));
  const char* length_pattern = "Content-Length: \([0-9]+\)";

  //Compilar la expresion regular
  ret = regcomp(cpattern,length_pattern,REG_EXTENDED);
  handle_regex_errors(ret);

  //Ejecutar la expresion regular
  ret = regexec(cpattern,http_header,2,matches,0);
  handle_regex_errors(ret);

  //Si se obtuvo el tamaño del contenido, reservar el espacio exacto
  if(ret != REG_NOMATCH){
    int length_start = matches[1].rm_so;
    int length_size =  matches[1].rm_eo - matches[1].rm_so;
    int clength = asciinum_to_int((http_header + length_start),
				  length_size);

    download_body_clength(sock_des, pg_ptr, clength,
			  url_header, url_header_sz);
  }
  //Si no se obtuvo el tamaño del contenido, descargar por bloques
  else{
    download_body_no_clength(sock_des, pg_ptr, 600,
			     url_header, url_header_sz);
  }


  regfree(cpattern);
  free(cpattern);
  free(matches);
  free(http_header);
  return 0;
}

char*
download_header(int sock_des){
  /*Lee el header HTTP en bloques de tamaño 'blck_sz'*/

  //Tamaño de bloque
  int blck_sz = 200;

  //Cantidad de bytes por leer del bloque actual
  int pending_sz = blck_sz;

  //Tamaño maximo del bufer. Crece con el numero de bloques que se agregan
  int max_buff_sz = blck_sz;

  //Tamaño actual del bufer. Cantidad de bytes en uso.
  int total_buff_sz = 0;

  //Reserva inicial de espacio para la pagina
  char* http_header = (char*)smalloc(sizeof(char)*(blck_sz + 1));

  //Apuntador donde escribira read()
  char* write_ptr = http_header;
  
  //bytes leidos en una llamada a read()
  ssize_t read_bytes = 0;

  //Arreglo usado para almacenar los ultimos 4 bytes leidos.
  //Sirve para detectar cuando se llega al final del header HTTP.
  char empty_line_detector[4];
  int i = 0;
  empty_line_detector[0] = '\0';
  empty_line_detector[1] = '\0';
  empty_line_detector[2] = '\0';
  empty_line_detector[3] = '\0';

  //Mientras read() lea algo que no sea una linea vacia
  while(!((empty_line_detector[0] == '\r')&&
	  (empty_line_detector[1] == '\n')&&
	  (empty_line_detector[2] == '\r')&&
	  (empty_line_detector[3] == '\n')
	  )&&
	((read_bytes = read(sock_des,write_ptr,1)) > 0)){

    //Actualizar detector de líneas vacías
    empty_line_detector[i] = (*write_ptr);
    i = (i+1) % 4;

    write_ptr += read_bytes;
    pending_sz -= read_bytes;
    total_buff_sz += read_bytes;

    //Si el tamaño del buffer alcanzÃ³ el mÃ¡ximo
    if(total_buff_sz == max_buff_sz){

      //Crear un nuevo bufer ('+ 1' por el byte nulo)
      char* t = (char*)smalloc(sizeof(char)*(total_buff_sz + blck_sz + 1));
      memcpy(t,http_header,total_buff_sz);

      //Actualizar apuntadores
      free(http_header);
      http_header = t;
      write_ptr = http_header+total_buff_sz;

      //Actualizar la cantidad de bytes por leer y el tamaño máximo
      pending_sz = blck_sz;
      max_buff_sz += blck_sz;
    }
  }

  if(read_bytes < 0){
    perror("Error leyendo del socket");
    exit(EXIT_FAILURE);
  }

  *(write_ptr) = '\0';//Byte nulo de terminación

  return http_header;
}

int download_body_clength(int sock_des, char** content,
			  int clength, char* url_header,
			  int url_header_sz){
  /*Descargar página web*/
  (*content) = (char*)smalloc(sizeof(char)*(url_header_sz + clength + 1));
  char* write_ptr = (*content);
  ssize_t pending_bytes = clength;
  ssize_t read_bytes = 0;

  //Añadir la cabecera para que el
  //parser sepa el URL de las entradas
  strcpy((*content),url_header);
  (*content)[url_header_sz-1] = '\n';
  write_ptr += url_header_sz;

  while((read_bytes = read(sock_des,write_ptr,pending_bytes)) > 0){
    write_ptr += read_bytes;
    pending_bytes -= read_bytes;
  }

  if(read_bytes < 0){
    perror("Error leyendo del socket");
    exit(EXIT_FAILURE);
  }

  *(write_ptr) = '\0';//Byte nulo de terminacion

  return 0;
}

int download_body_no_clength(int sock_des, char** content,
			     int blck_sz, char* url_header,
			     int url_header_sz){
  /*Lee el header HTTP en bloques de tamaño 'blk_sz'*/

  //Cantidad de bytes por leer del bloque actual
  int pending_sz = blck_sz;

  //Tamaño maximo del bufer. Crece con el numero de bloques que se agregan
  int max_buff_sz = blck_sz;

  //Tamaño actual del bufer. Cantidad de bytes en uso.
  int total_buff_sz = 0;


  //Reserva inicial de espacio para la página
  char* http_header = (char*)smalloc(sizeof(char)*(url_header_sz +
						   blck_sz + 1));

  //Apuntador donde escribira read()
  char* write_ptr = (*content);

  //bytes leidos en una llamada a read()
  ssize_t read_bytes = 0;

  //Añadir la cabecera para que el
  //parser sepa el URL de las entradas
  strcpy((*content),url_header);
  (*content)[url_header_sz-1] = '\n';
  write_ptr += url_header_sz;
  
  //Mientras read() lea algo que no sea una linea vacía
  while((read_bytes = read(sock_des,write_ptr,pending_sz)) > 0){

    write_ptr += read_bytes;
    pending_sz -= read_bytes;
    total_buff_sz += read_bytes;

    //Si el tamaño del buffer alcanzÃ³ el mÃ¡ximo
    if(total_buff_sz == max_buff_sz){

      //Crear un nuevo bufer ('+ 1' por el byte nulo)
      char* t = (char*)smalloc(sizeof(char)*(total_buff_sz + blck_sz + 1));
      memcpy(t,(*content),total_buff_sz);

      //Actualizar apuntadores
      free(*content);
      (*content) = t;
      write_ptr = (*content) + total_buff_sz;

      //Actualizar la cantidad de bytes por leer y el tamaño máximo
      pending_sz = blck_sz;
      max_buff_sz += blck_sz;
    }
  }

  if(read_bytes < 0){
    perror("Error leyendo del socket");
    exit(EXIT_FAILURE);
  }

  *(write_ptr) = '\0';//Byte nulo de terminación

  return 0;
}
